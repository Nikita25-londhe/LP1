#Implementation Of Basic GUI using pytjon tKinter Library
import tkinter as tk
from tkinter import messagebox
def edit_button():
    def save_button():
        updated=input.get()
        label.config(text=f"updated value is {updated}")
        dialog_window.destroy()

    dialog_window=tk.Toplevel(root)
    dialog_window.geometry("400x300")
    dialog_window.title("Edit Dialgo Box")

    input=tk.Entry(dialog_window,width=20)
    input.pack(pady="20")

    btn_save=tk.Button(dialog_window,text="save",command=save_button,bg="green",fg="Yellow")
    btn_save.pack(pady="20")
    
root=tk.Tk()
root.title("Basic Implementation of GUI")
root.geometry("500x400")

label=tk.Label(text="Welcome to My App",bg="yellow",fg="red",font=("arial",20))
label.pack(pady="20")

edit_btn=tk.Button(root,command=edit_button,bg="green",fg="yellow",text="Edit")
edit_btn.pack(pady="10")

root.mainloop()
