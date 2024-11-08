import tkinter as tk
from tkinter import ttk,messagebox
def login_func():
    username=usr_entry.get()
    password=pass_entry.get()
    if(username=="Nikita"  and password=="123"):
        messagebox.showinfo("Welocme!! Logged in Successfully")
        enable_edit()
        tab_control.select(edit_tab)
        tab_control.forget(login_tab)
    else:
        messagebox.showerror("Invalid username or password")
def enable_edit():
    tab_control.tab(1,state="normal")
def edit_func():
   input=edit_entry.get()
   label.configure(text=f"Updated value is: {input} ")    
root=tk.Tk()
root.geometry("500x400")
root.title("Redesign of UI")

tab_control=ttk.Notebook(root)
login_tab=ttk.Frame(tab_control)

tab_control.add(login_tab,text="Login")
usr_label=tk.Label(login_tab,text="Enter Username")
usr_label.pack(pady="10")
usr_entry=tk.Entry(login_tab,width="20")
usr_entry.pack(pady="10")

pass_label=tk.Label(login_tab,text="Enter password")
pass_label.pack(pady="10")
pass_entry=tk.Entry(login_tab,width="20")
pass_entry.pack(pady="10")

login_btn=tk.Button(login_tab,text="Login",bg="Red",fg="Yellow",command=login_func)
login_btn.pack(pady="20")
edit_tab=ttk.Frame(tab_control)
tab_control.add(edit_tab,text="Edit")

label=tk.Label(edit_tab,text="Welcome to My App",bg="yellow",fg="red")
label.pack(pady="20")

edit_entry=tk.Entry(edit_tab,width="20")
edit_entry.pack(pady="20")

edit_btn=tk.Button(edit_tab,text="Edit",bg="Red",fg="yellow",command=edit_func)
edit_btn.pack(pady=10)

tab_control.pack(expand=1,fill="both")
tab_control.tab(1,state="disabled")
root.mainloop()
