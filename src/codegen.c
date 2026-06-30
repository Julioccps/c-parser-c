#include "codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int find_var_decl_in_block(ASTNode* block, const char* var_name) {
    if (!block) return 0;
    if (block->type == NODE_VAR_DECL && strcmp(block->token->value, var_name) == 0) {
        return 1;
    }
    for (int i = 0; i < block->child_count; i++) {
        if (find_var_decl_in_block(block->children[i], var_name)) {
            return 1;
        }
    }
    return 0;
}

static const char* resolve_scope(ASTNode* func_node, const char* var_name) {
    if (!func_node) return "global";
    
    for (int i = 0; i < func_node->child_count; i++) {
        ASTNode* child = func_node->children[i];
        if (child->type == NODE_PARAM && strcmp(child->token->value, var_name) == 0) {
            return func_node->token->value;
        }
    }
    
    for (int i = 0; i < func_node->child_count; i++) {
        ASTNode* child = func_node->children[i];
        if (child->type == NODE_BLOCK) {
            if (find_var_decl_in_block(child, var_name)) {
                return func_node->token->value;
            }
        }
    }
    
    return "global";
}

static void compile_expr(ASTNode* node, ASTNode* func_node, FILE* out) {
    if (!node) return;
    
    if (node->type == NODE_LITERAL) {
        if (node->token->type == TOKEN_IDENTIFIER) {
            const char* scope = resolve_scope(func_node, node->token->value);
            fprintf(out, "push %s:%s\n", scope, node->token->value);
        } else {
            fprintf(out, "push %s\n", node->token->value);
        }
    } else if (node->type == NODE_BINARY_OP) {
        compile_expr(node->children[0], func_node, out);
        compile_expr(node->children[1], func_node, out);
        if (strcmp(node->token->value, "+") == 0) {
            fprintf(out, "add\n");
        } else if (strcmp(node->token->value, "-") == 0) {
            fprintf(out, "subt\n");
        } else if (strcmp(node->token->value, "*") == 0) {
            fprintf(out, "mul\n");
        } else if (strcmp(node->token->value, "/") == 0) {
            fprintf(out, "div\n");
        } else if (strcmp(node->token->value, "%") == 0) {
            fprintf(out, "mod\n");
        }
    } else if (node->type == NODE_FUNC_CALL) {
        ASTNode* target = node->children[0];
        const char* name = target->token->value;
        if (strcmp(name, "print") == 0) {
            compile_expr(node->children[1], func_node, out);
            static int print_temp_idx = 0;
            const char* scope = func_node ? func_node->token->value : "global";
            fprintf(out, "pop %s:print_temp_%d\n", scope, print_temp_idx);
            
            const char* type = "int";
            if (node->children[1]->type == NODE_LITERAL && node->children[1]->token->type == TOKEN_LITERAL_STRING) {
                type = "string";
            }
            fprintf(out, "print %s %s:print_temp_%d\n", type, scope, print_temp_idx);
            print_temp_idx++;
        } else {
            for (int i = 1; i < node->child_count; i++) {
                compile_expr(node->children[i], func_node, out);
            }
            fprintf(out, "sub %s()\n", name);
        }
    }
}

static void compile_block(ASTNode* node, ASTNode* func_node, FILE* out) {
    if (!node) return;
    for (int i = 0; i < node->child_count; i++) {
        ASTNode* stmt = node->children[i];
        if (!stmt) continue;
        
        if (stmt->type == NODE_VAR_DECL) {
            const char* scope = func_node ? func_node->token->value : "global";
            const char* var_name = stmt->token->value;
            const char* default_val = "0";
            
            if (stmt->child_count > 0) {
                ASTNode* init = stmt->children[0];
                if (init->type == NODE_LITERAL) {
                    if (init->token->type == TOKEN_IDENTIFIER) {
                        const char* init_scope = resolve_scope(func_node, init->token->value);
                        static char var_ref[256];
                        snprintf(var_ref, sizeof(var_ref), "%s:%s", init_scope, init->token->value);
                        default_val = var_ref;
                    } else {
                        default_val = init->token->value;
                    }
                    fprintf(out, "set %s:%s int %s\n", scope, var_name, default_val);
                } else {
                    fprintf(out, "set %s:%s int 0\n", scope, var_name);
                    compile_expr(init, func_node, out);
                    fprintf(out, "pop %s:%s\n", scope, var_name);
                }
            } else {
                fprintf(out, "set %s:%s int 0\n", scope, var_name);
            }
        } else if (stmt->type == NODE_ASSIGN) {
            const char* scope = func_node ? func_node->token->value : "global";
            const char* var_name = stmt->token->value;
            compile_expr(stmt->children[0], func_node, out);
            fprintf(out, "pop %s:%s\n", scope, var_name);
        } else if (stmt->type == NODE_RETURN) {
            const char* scope = func_node ? func_node->token->value : "global";
            if (func_node && strcmp(func_node->token->value, "main") == 0) {
                fprintf(out, "ret void\n");
            } else {
                if (stmt->child_count > 0) {
                    compile_expr(stmt->children[0], func_node, out);
                    fprintf(out, "pop %s:ret_val\n", scope);
                    fprintf(out, "ret int %s:ret_val\n", scope);
                } else {
                    fprintf(out, "ret void\n");
                }
            }
        } else if (stmt->type == NODE_FUNC_CALL) {
            compile_expr(stmt, func_node, out);
        } else if (stmt->type == NODE_BLOCK) {
            compile_block(stmt, func_node, out);
        }
    }
}

void generate_bytecode(ASTNode* ast, const char* output_filename) {
    FILE* out = fopen(output_filename, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_filename);
        return;
    }
    
    for (int i = 0; i < ast->child_count; i++) {
        ASTNode* child = ast->children[i];
        if (child->type == NODE_VAR_DECL) {
            const char* var_name = child->token->value;
            const char* default_val = "0";
            if (child->child_count > 0 && child->children[0]->type == NODE_LITERAL) {
                default_val = child->children[0]->token->value;
            }
            fprintf(out, "set global:%s int %s\n", var_name, default_val);
        }
    }
    
    ASTNode* main_node = NULL;
    for (int i = 0; i < ast->child_count; i++) {
        ASTNode* child = ast->children[i];
        if (child->type == NODE_FUNC_DEF) {
            if (strcmp(child->token->value, "main") == 0) {
                main_node = child;
            } else {
                const char* func_name = child->token->value;
                const char* ret_type = child->children[0]->token->value;
                fprintf(out, "fun %s %s", ret_type, func_name);
                
                int child_idx = 1;
                while (child_idx < child->child_count && child->children[child_idx]->type == NODE_PARAM) {
                    ASTNode* param = child->children[child_idx];
                    fprintf(out, " param %s %s", param->children[0]->token->value, param->token->value);
                    child_idx++;
                }
                fprintf(out, "\n");
                
                ASTNode* block = child->children[child_idx];
                compile_block(block, child, out);
                
                int has_ret = 0;
                if (block && block->child_count > 0) {
                    ASTNode* last_stmt = block->children[block->child_count - 1];
                    if (last_stmt->type == NODE_RETURN) has_ret = 1;
                }
                if (!has_ret) {
                    if (strcmp(ret_type, "void") == 0) {
                        fprintf(out, "ret void\n");
                    } else {
                        fprintf(out, "set %s:ret_val int 0\n", func_name);
                        fprintf(out, "ret int %s:ret_val\n", func_name);
                    }
                }
                fprintf(out, "\n");
            }
        }
    }
    
    if (main_node) {
        fprintf(out, "fun void main\n");
        int child_idx = 1;
        while (child_idx < main_node->child_count && main_node->children[child_idx]->type == NODE_PARAM) {
            child_idx++;
        }
        ASTNode* block = main_node->children[child_idx];
        compile_block(block, main_node, out);
        
        int has_ret = 0;
        if (block && block->child_count > 0) {
            ASTNode* last_stmt = block->children[block->child_count - 1];
            if (last_stmt->type == NODE_RETURN) has_ret = 1;
        }
        if (!has_ret) {
            fprintf(out, "ret void\n");
        }
    } else {
        fprintf(out, "fun void main\nret void\n");
    }
    
    fclose(out);
}
