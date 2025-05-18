import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess

def browse_file():
    file_path = filedialog.askopenfilename(filetypes=[("C files", "*.c")])
    if file_path:
        file_entry.delete(0, tk.END)
        file_entry.insert(0, file_path)

def analyze_file():
    file_path = file_entry.get()
    if not file_path:
        messagebox.showerror("Error", "Please select a file.")
        return
    
    try:
        # Run the C++ executable and capture output
        result = subprocess.run(["main1.exe", file_path], capture_output=True, text=True)
        if result.returncode != 0:
            messagebox.showerror("Error", result.stderr)
            return
        
        output_text.delete("1.0", tk.END)
        output_text.insert(tk.END, result.stdout)

    except Exception as e:
        messagebox.showerror("Exception", str(e))

# Tkinter UI
root = tk.Tk()
root.title("Lexical Analyzer UI")
root.geometry("700x500")

frame = tk.Frame(root)
frame.pack(pady=10)

tk.Label(frame, text="Select C File:").grid(row=0, column=0, padx=5)
file_entry = tk.Entry(frame, width=50)
file_entry.grid(row=0, column=1, padx=5)
tk.Button(frame, text="Browse", command=browse_file).grid(row=0, column=2, padx=5)

tk.Button(root, text="Analyze", command=analyze_file, width=20, bg="lightblue").pack(pady=10)

output_text = tk.Text(root, wrap=tk.WORD, font=("Courier", 12))
output_text.pack(expand=True, fill=tk.BOTH, padx=10, pady=10)

root.mainloop()
