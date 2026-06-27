import lark
import sys

class stack():
    def __init__(self):
        self.stack = []
        self.top = -1
    
    def push(self, value):
        self.stack.append(value)
        self.top += 1

    def pop(self):
        if self.top >= 0:
            value = self.stack.pop()
            self.top = len(self.stack) - 1
            return value
        else:
            raise IndexError("pop from empty stack")
    
    def is_empty(self):
        return self.top == -1
    
    def peek(self):
        if self.top >= 0:
            return self.stack[self.top]
        else:
            raise IndexError("peek from empty stack")

class interpreter():
    def __init__(self):
        self.stack = stack()
        self.symbol_table = {}
        self.variable_table = {}

    def execute(self, tree):
        nodes = tree.children if hasattr(tree, 'children') else tree
        for child in nodes:
            if hasattr(child, 'data'):
                node = child.children[0] if child.data == 'statement' else child
                
                if node.data == 'main':
                    self.execute(node.children)
                elif node.data == 'function':
                    self.handle_function(node)
                elif node.data == 'call':
                    self.handle_call(node)
                elif node.data == 'definition':
                    self.handle_definition(node)
                elif node.data == 'attribution':
                    self.handle_attribution(node)
                elif node.data == 'stack':
                    self.handle_stack(node)
                elif node.data == 'op':
                    self.handle_op(node)
                elif node.data == 'print':
                    if len(node.children) == 2:
                        value_node = node.children[1]
                    else:
                        value_node = node.children[1:]
                    value = self._resolve_value(value_node)
                    print(value)
                elif node.data == 'return_stmt':
                    self.handle_return(node)

    def handle_function(self, node):
        func_name = node.children[1].value
        params = []
        if len(node.children) > 2 and hasattr(node.children[2], 'data') and node.children[2].data == 'params':
            for param in node.children[2].children:
                params.append({
                    "type": param.children[0].value,
                    "name": param.children[1].value,
                })

        statements = [c for c in node.children if hasattr(c, 'data') and (c.data == 'statement' or c.data == 'return_stmt')]
        
        self.symbol_table[func_name] = {
            "type": node.children[0].value,
            "params": params,
            "body": statements,
        }

    def handle_call(self, node):
        func_name = node.children[0].value
        if func_name in self.symbol_table:
            func_data = self.symbol_table[func_name]
            params = func_data.get("params", [])
            arg_values = [self.stack.pop() for _ in range(len(params))]

            for param, value in zip(params, reversed(arg_values)):
                var_key = f"{func_name}:{param['name']}"
                self.variable_table[var_key] = value

            self.execute(func_data["body"])
        else:
            raise NameError(f"Function '{func_name}' is not defined.")

    def handle_definition(self, node):
        scope = node.children[0].value
        name = node.children[1].value
        var_key = f"{scope}:{name}"
        
        raw_val = node.children[3]
        self.variable_table[var_key] = self._resolve_value(raw_val)

    def handle_attribution(self, node):
        scope = node.children[0].value
        name = node.children[1].value
        var_key = f"{scope}:{name}"
        
        raw_val = node.children[2]
        self.variable_table[var_key] = self._resolve_value(raw_val)

    def handle_stack(self, node):
        action = node.children[0]
        
        if action.type == "PUSH":
            raw_val = node.children[1] if len(node.children) == 2 else node.children[1:3]
            val_to_push = self._resolve_value(raw_val)
            self.stack.push(val_to_push)
        
        elif action.type == "POP":
            scope = node.children[1].value
            name = node.children[2].value
            var_key = f"{scope}:{name}"
            
            popped_val = self.stack.pop()
            self.variable_table[var_key] = popped_val

    def handle_op(self, node):
        if not node.children:
            raise ValueError(f"Empty op node: {node}")
        operator = node.children[0]
        
        b = self.stack.pop()
        a = self.stack.pop()
        
        if operator.type == "ADD": result = int(a) + int(b)
        elif operator.type == "SUBT": result = int(a) - int(b)
        elif operator.type == "MUL": result = int(a) * int(b)
        elif operator.type == "DIV": result = int(a) // int(b)
        elif operator.type == "MOD": result = int(a) % int(b)
        else:
            raise ValueError(f"Unknown operator {operator}")
        
        self.stack.push(result)

    def handle_return(self, node):
        if len(node.children) > 1:
            raw_val = node.children[1] if len(node.children) == 2 else node.children[1:]
            return_value = self._resolve_value(raw_val)
            self.stack.push(return_value)

    def _resolve_value(self, token_or_node):
        if isinstance(token_or_node, lark.Token):
            if token_or_node.type == 'NUMBER':
                return int(token_or_node.value)
            return token_or_node.value.strip('"')
        
        if hasattr(token_or_node, 'children'):
            pass
        
        if isinstance(token_or_node, list) or hasattr(token_or_node, 'data'):
            children = token_or_node.children if hasattr(token_or_node, 'data') else token_or_node
            scope = children[0].value
            name = children[1].value
            var_key = f"{scope}:{name}"
            return self.variable_table.get(var_key, 0)

        return token_or_node

grammar = r"""
program: 	 definition* function* call* main
function: "fun" TYPE NAME params statement* return_stmt
params: (param)*
param: "param" TYPE NAME
return_stmt: "ret" TYPE (NAME | SCOPE ":" NAME)?
statement:  definition | attribution | call | function | stack | op | print
definition: "set" SCOPE ":" NAME TYPE (NUMBER | STRING | SCOPE ":" NAME)
attribution: "set" SCOPE ":" NAME (NUMBER | STRING | SCOPE ":" NAME)
call: "sub" NAME "()"
print: "print" TYPE (SCOPE ":" NAME | NUMBER | STRING)
stack: PUSH (SCOPE ":" NAME | NUMBER) | POP SCOPE ":" NAME
op: ADD | SUBT | MUL | DIV | MOD
main: "fun" "void" "main" statement* "ret" "void"
TYPE: "int" | "void" | "string" | "auto"
STRING: /"[^"]*"/
SCOPE: /\w+/
NAME:   /\w+/
NUMBER: /\d+/
ADD: "add"
SUBT: "subt"
MUL: "mul"
DIV: "div"
MOD: "mod"
PUSH: "push"
POP: "pop"
%ignore /[ \t\f\r\n]+/
"""

def main():
    parser = lark.Lark(grammar, start='program')
    interpreter_instance = interpreter()

    if len(sys.argv) < 2:
        print("Usage: python interpreter.py <source_file>")
        sys.exit(1)
    else:
        source_file = sys.argv[1]
        with open(source_file, 'r') as f:
            source_code = f.read()
        
        try:
            parse_tree = parser.parse(source_code)
            interpreter_instance.execute(parse_tree)
        except lark.exceptions.LarkError as e:
            print(f"Parsing error: {e}")
        except Exception as e:
            print(f"Execution error: {e}")

if __name__ == "__main__":
    main()