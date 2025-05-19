'''import tkinter as tk

# Parse Tree Node Class
class ParseTreeNode:
    def __init__(self, symbol):
        self.symbol = symbol
        self.children = []

# Parser Class (Recursive Descent)
class RecursiveParser:
    def __init__(self, input_string):
        self.input = input_string
        self.pos = 0

    def parse(self):
        root = self.S()
        if root and self.pos == len(self.input):
            return root
        else:
            print("Error: Invalid input or input not fully consumed.")
            return None

    def S(self):
        node = ParseTreeNode("S")
        if self.match('c'):
            node.children.append(ParseTreeNode('c'))
            A_node = self.A()
            if A_node:
                node.children.append(A_node)
                if self.match('d'):
                    node.children.append(ParseTreeNode('d'))
                    return node
        return None

    def A(self):
        node = ParseTreeNode("A")
        if self.match('a'):
            node.children.append(ParseTreeNode('a'))
            if self.match('b'):
                node.children.append(ParseTreeNode('b'))
                return node
            else:
                return node  # Just 'a' (matches A -> a)
        return None

    def match(self, expected_char):
        if self.pos < len(self.input) and self.input[self.pos] == expected_char:
            self.pos += 1
            return True
        return False

# GUI Visualizer
class ParseTreeVisualizer:
    def __init__(self, root_node):
        self.root_node = root_node
        self.window = tk.Tk()
        self.window.title("Parse Tree Visualizer")
        self.canvas = tk.Canvas(self.window, width=800, height=600, bg='white')
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.y_gap = 80
        self.draw_tree(self.root_node, 400, 20, 200)

    def draw_tree(self, node, x, y, x_range):
        if node is None:
            return
        self.canvas.create_text(x, y, text=node.symbol, font=("Arial", 12, "bold"))
        if not node.children:
            return
        gap = x_range // max(2, len(node.children))
        start_x = x - x_range // 2 + gap // 2
        for i, child in enumerate(node.children):
            child_x = start_x + i * gap
            child_y = y + self.y_gap
            self.canvas.create_line(x, y + 10, child_x, child_y - 10)
            self.draw_tree(child, child_x, child_y, gap)

    def run(self):
        self.window.mainloop()

# Main Function
def main():
    input_string = input("Enter input string to parse: ")
    parser = RecursiveParser(input_string)
    root = parser.parse()

    if root:
        print("Input parsed successfully!")
        visualizer = ParseTreeVisualizer(root)
        visualizer.run()
    else:
        print("Parsing failed.")

if __name__ == "__main__":
    main()
'''

import tkinter as tk

class ParseTreeNode:
    def __init__(self, symbol):
        self.symbol = symbol
        self.children = []

class RecursiveDescentParser:
    def __init__(self, grammar, input_tokens):
        self.grammar = grammar
        self.input = input_tokens
        self.pos = 0

    def parse(self, start_symbol):
        self.pos = 0
        root = self._parse_non_terminal(start_symbol)
        if root and self.pos == len(self.input):
            return root
        return None

    def _parse_non_terminal(self, symbol):
        if symbol not in self.grammar:
            return None

        for production in self.grammar[symbol]:
            saved_pos = self.pos
            node = ParseTreeNode(symbol)
            children = []

            if production == ['ε'] or production == ['epsilon']:
                children.append(ParseTreeNode('ε'))
                node.children.extend(children)
                return node

            success = True
            for sym in production:
                if sym in self.grammar:
                    child_node = self._parse_non_terminal(sym)
                    if child_node:
                        children.append(child_node)
                    else:
                        success = False
                        break
                else:
                    if self.match(sym):
                        children.append(ParseTreeNode(sym))
                    else:
                        success = False
                        break

            if success:
                node.children.extend(children)
                return node

            self.pos = saved_pos

        return None

    def match(self, expected_char):
        if self.pos < len(self.input) and self.input[self.pos] == expected_char:
            self.pos += 1
            return True
        return False

class ParseTreeVisualizer:
    def __init__(self, root_node):
        self.root_node = root_node
        self.window = tk.Tk()
        self.window.title("Parse Tree Visualizer")
        self.canvas = tk.Canvas(self.window, width=1200, height=800, bg='white')  # bigger canvas
        self.canvas.pack(fill=tk.BOTH, expand=True)
        self.y_gap = 100  # more vertical space
        self.draw_tree(self.root_node, 600, 20, 600)  # bigger initial x_range

    def draw_tree(self, node, x, y, x_range):
        if node is None:
            return
        self.canvas.create_text(x, y, text=node.symbol, font=("Arial", 12, "bold"))
        if not node.children:
            return
        # Increase gap to allocate more space per child
        gap = max(50, x_range // max(1, len(node.children)))  # minimum 50 pixels gap
        # Start x position so children are centered under parent
        start_x = x - (gap * (len(node.children) - 1)) // 2
        for i, child in enumerate(node.children):
            child_x = start_x + i * gap
            child_y = y + self.y_gap
            self.canvas.create_line(x, y + 10, child_x, child_y - 10)
            self.draw_tree(child, child_x, child_y, gap)

    def run(self):
        self.window.mainloop()

def main():
    grammar = {
        'E': [['T', 'Ep']],
        'Ep': [['+', 'T', 'Ep'], ['ε']],
        'T': [['F', 'Tp']],
        'Tp': [['*', 'F', 'Tp'], ['ε']],
        'F': [['n']]
    }

    input_string = input("Enter input string to parse: ")
    tokens = list(input_string.replace(" ", ""))
    parser = RecursiveDescentParser(grammar, tokens)
    root = parser.parse('E')

    if root:
        print("Input parsed successfully!")
        visualizer = ParseTreeVisualizer(root)
        visualizer.run()
    else:
        print("Parsing failed or invalid input.")

if __name__ == "__main__":
    main()
