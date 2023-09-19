from tkinter import *
from tkinter.ttk import *
from tkinter.messagebox import showinfo
from tkinter import Tk, Button, Frame, Entry, END
from PIL import Image, ImageTk
from tkinter.filedialog import *
import os

fenetre = Tk()
fenetre.winfo_toplevel().title("Compression Basée Super-pixels")
label = Entry(fenetre)
label.pack(side="top", fill="x")


"""
def ouvrir():
   fenetre.quit
   filepath = askopenfilename(title="Ouvrir une image",filetypes=[('png files','.png'),('all files','.*')])
   image = Image.open(filepath)
   if image.width>=2000 or image.height>=1080 : resize_image = image.resize((int(image.width*0.5),int(image.height*0.5)))
   else : resize_image = image 
   test = ImageTk.PhotoImage(resize_image)

   label1 = Label(fenetre,image=test)
   label1.image = test

   # Position image
   label1.place(x=0, y=0)
   if image.width>=2000 or image.height>=1080 :
      w = int(image.width*0.5)
      h = int(image.height*0.5)
   else :
      w = image.width
      h = image.height
   fenetre.geometry(f"{w}x{h}")
   fenetre.mainloop()
def slic():
   #showinfo("Traitement", "Traitement en cours")
   #os.system("hello")
   filepath = askopenfilename(title="Ouvrir une image",filetypes=[('png files','.png'),('all files','.*')])
   image = Image.open(filepath)
   if image.width>=2000 or image.height>=1080 : resize_image = image.resize((int(image.width*0.5),int(image.height*0.5)))
   else : resize_image = image 
   test = ImageTk.PhotoImage(resize_image)

   label1 = Label(fenetre,image=test)
   label1.image = test

   # Position image
   label1.place(x=0, y=0)
   if image.width>=2000 or image.height>=1080 :
      w = int(image.width*0.5)
      h = int(image.height*0.5)
   else :
      w = image.width
      h = image.height
   fenetre.geometry(f"{w}x{h}")
   fenetre.mainloop()
   new_name = filename.rsplit(".",1)[0] + "SUPERGENIAL.pgm"
   os.system("test_grey" + filepath + new_name + "120")

menubar = Menu(fenetre)

menu1 = Menu(menubar, tearoff=0)
menu1.add_command(label="Ouvrir", command=ouvrir)
menu1.add_command(label="SLIC", command=slic)
menu1.add_separator()
menu1.add_command(label="Quitter", command=fenetre.quit)
menubar.add_cascade(label="Fichier", menu=menu1)


fenetre.config(menu=menubar)
"""

menubar = Menu(fenetre)

menu1 = Menu(menubar, tearoff=0)
menu1.add_command(label="Quitter", command=fenetre.quit)
menubar.add_cascade(label="Fichier", menu=menu1)

fenetre.config(menu=menubar)

filepath = askopenfilename(title="Ouvrir une image",filetypes=[('ppm files','.ppm'),('all files','.*')])
"""
image = Image.open(filepath)
resize_image = image.resize((int(image.width*0.5),int(image.height*0.5)))
test = ImageTk.PhotoImage(resize_image)

label1 = Label(fenetre,image=test)
label1.image = test

# Position image
label1.place(x=0, y=0)
w = int(image.width*0.5)
h = int(image.height*0.5)
fenetre.geometry(f"{w}x{h}")
"""
file_nom = filepath.rsplit("/",1)[-1]
new_name = file_nom.rsplit(".",1)[0] + "SLIC.ppm"
new_name_comp = file_nom.rsplit(".",1)[0] + "Comp.ppm"
new_name_palet = file_nom.rsplit(".",1)[0] + "Palet.ppm"
new_name_index = file_nom.rsplit(".",1)[0] + "Index.pgm"

print("file_nom = " + file_nom)
print("new_name = " + new_name)
print("new_name_comp = " + new_name_comp)
if(not os.path.exists("./out")):
      os.makedirs("./out")
#Version LinuX
if os.system("./main " + filepath + " ./out/" + new_name + " 200 " + "3") != 0:
   print("An error occured.") 

else:
   #os.system("cd palet/")
   #os.system("ls")
   
   os.system("./palet ./out/" + new_name + " ./out/" + new_name_comp + " 256 " + " ./out/" + new_name_palet + " ./out/" + new_name_index)

   #os.system("test_grey " + file_nom + " " + new_name + " 120") #Version Windows
   #os.system("slic " + file_nom + " " + new_name)
   #os.system("./measurment " + new_name + " " + new_name_comp)


   f_p = "out/"
   f_p2 = f_p + file_nom.rsplit(".",1)[0] + "Index.pgm"
   new_path = filepath.rsplit(".",1)[0] + "Index.pgm"
   f_p3 = "./" + f_p2
   #new_path = f_p2
   print("new_path = " + new_path)
   print("f_p3 = " + f_p3)
   image2 = Image.open(f_p3)
   if image2.width>=1900 or image2.height>=1000 : resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
   else : resize_image2 = image2 
   #resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
   test2 = ImageTk.PhotoImage(resize_image2)

   label2 = Label(fenetre,image=test2)
   label2.image = test2

   # Position image
   label2.place(x=0, y=0)
   if image2.width>=1900 or image2.height>=1000 :
      w = int(image2.width*0.5)
      h = int(image2.height*0.5)
   else :
      w = image2.width
      h = image2.height
   fenetre.geometry(f"{w}x{h}")
fenetre.mainloop()
