import tkinter as tk
from tkinter import messagebox,ttk
def sign():
    messagebox.showinfo("Signed up successfully!!")
    tab_controls.select(login_tab)
def login():
    username=usr_entry.get()
    password=pass_entry.get()
    if(username=="Nikita"  and password=="123"):
        messagebox.showinfo("Welocme!! Logged in Successfully")
        enable()
        tab_controls.forget(login_tab)
        tab_controls.forget(sign_up)
    else:
        messagebox.showerror("Invalid username or password")
def enable():
    tab_controls.tab(2,state="normal")
    tab_controls.tab(3,state="normal")
root=tk.Tk()
root.geometry("800x600")
root.title("PICT HOSTEL MANAGEMENT SYSTEM")
root.configure(bg="lightgray")
tab_controls=ttk.Notebook(root)
sign_up=ttk.Frame(tab_controls)
login_tab=ttk.Frame(tab_controls)
home_tab=ttk.Frame(tab_controls)
guidelines=ttk.Frame(tab_controls)
tab_controls.add(sign_up,text="Sign Up")
tab_controls.add(login_tab,text="Login")
tab_controls.add(home_tab,text="Home page",state="disabled")
tab_controls.add(guidelines,text="Guidelines",state="disabled")
fields=["Name","Branch","Year","username","password"]
for i in fields:
    tk.Label(sign_up,text=i,font="arial").pack(pady=5)
    input=tk.Entry(sign_up,width=40)
    if i=="password":
        input.config(show="*")
    input.pack(pady=5)
sign_up_btn=tk.Button(sign_up,text="Sign Up",bg="red",fg="yellow",font="arial",command=sign)
sign_up_btn.pack(pady=10)

usr_label=tk.Label(login_tab,text="Enter Username",font="arial")
usr_label.pack(pady="10")
usr_entry=tk.Entry(login_tab,width="20",font="arial")
usr_entry.pack(pady="10")

pass_label=tk.Label(login_tab,text="Enter password",font="arial")
pass_label.pack(pady="10")
pass_entry=tk.Entry(login_tab,width="20",font="arial")
pass_entry.config(show="*")
pass_entry.pack(pady="10")

login_btn=tk.Button(login_tab,text="Login",bg="Red",fg="Yellow",command=login,font="arial")
login_btn.pack(pady="20")
tk.Label(home_tab,text="PICT Hostel Management System",bg="blue",fg="white",font="arial").pack(pady=20)
features="""
1 24/7 Security
2 Unlimites WiFI
3 Lunch and Dinner with breakfast
4 High class amenities
5 Hot Water 
6 Medical checkup once in week
7 Daily Attendance
8 Admission On the basis of CGPA"""
tk.Label(home_tab,text=features,bg="lightblue").pack(pady=20)
tk.Label(guidelines,text="Guidelines",bg="blue",fg="white",font="arial").pack(pady=20)
gui="""
1. Students must adhere to hostel timings strictly.
2. Visitors are allowed only during specified hours.
3. No loud music or noise after 10 PM.
4. Maintain cleanliness in rooms and common areas.
5. Any damage to hostel property must be reported immediately.
6. Ragging in any form is strictly prohibited.
    """
tk.Label(guidelines,text=gui,bg="lightblue").pack(pady=20)
tab_controls.pack(expand=1,fill="both")


root.mainloop()
