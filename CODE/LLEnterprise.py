from tkinter import *
from tkinter.messagebox import showinfo
from tkinter import Tk, Button, Frame, Entry, END
from PIL import Image, ImageTk
from tkinter.filedialog import *
import os
fenetre = Tk()
class Demo1:
    def __init__(self, master, k, m):
        filepath = askopenfilename(title="Ouvrir une image",filetypes=[('png files','.png'),('all files','.*')])
        file_nom = filepath.rsplit("/",1)[-1]
        new_name = file_nom.rsplit(".",1)[0] + "SLIC.ppm"
        new_name_comp = file_nom.rsplit(".",1)[0] + "Comp.ppm"
        new_name_palet = file_nom.rsplit(".",1)[0] + "Palet.ppm"
        new_name_index = file_nom.rsplit(".",1)[0] + "Index.pgm"

        print("file_nom = " + file_nom)
        print("new_name = " + new_name)
        print("new_name_comp = " + new_name_comp)
        new_k = k.get()
        new_m = m.get()
        if(not int(k.get()) in (200, 300, 400, 750, 1000)): 
            new_k = "1000"
        if(not int(m.get()) in (1, 3, 10, 15, 40)):
        	new_m = "15"

        # os.system("./main " + file_nom + " " + new_name + k.get() + m.get()) #Version LinuX
        # os.chdir("palet/")
        # os.system("./palet ../" + new_name + " ../" + new_name_comp + " 256 " + new_name_palet + " " + new_name_index)
        if(not os.path.exists("./out")):
            os.makedirs("./out")
        #Version LinuX
        if os.system("./main " + filepath + " ./out/" + new_name + " " + new_k + " "+ new_m) != 0:
           print("An error occured. Try with different paramters.")
           return

        os.system("./palet ./out/" + new_name + " ./out/" + new_name_comp + " 256 " + " ./out/" + new_name_palet + " ./out/" + new_name_index)

        f_p = "out/"
        f_p2 = f_p + file_nom.rsplit(".",1)[0] + "Index.pgm"
        new_path = filepath.rsplit(".",1)[0] + "Index.pgm"
        f_p3 = "./" + f_p2
        #new_path = f_p2
        f_p4 = f_p + file_nom.rsplit(".",1)[0] + "SLIC.ppm"
        print("new_path = " + new_path)
        print("f_p3 = " + f_p3)
        #f_p3 for comp et fp_4 for slic
        image2 = Image.open(f_p4)
        if image2.width>=1900 or image2.height>=1000 : resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
        else : resize_image2 = image2 
        #resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
        test2 = ImageTk.PhotoImage(resize_image2)

        label2 = Label(master,image=test2)
        label2.image = test2

        # Position image
        label2.place(x=0, y=0)
        if image2.width>=1900 or image2.height>=1000 :
           w = int(image2.width*0.5)
           h = int(image2.height*0.5)
        else :
           w = image2.width
           h = image2.height
        master.geometry(f"{w}x{h}")
        self.master = master
        self.frame = Frame(self.master)
        self.button1 = Button(self.frame, text = 'New Window', width = 25, command = self.new_window)
        #self.button1.pack()
        #self.button1.place(x=0, y=512)
        self.frame.pack()

    def new_window(self):
        self.newWindow = Toplevel(self.master)
        #self.master.destroy()
        self.app = Demo2(self.newWindow)

class Demo2:
    def __init__(self, master):
        self.master = master
        self.frame = Frame(self.master)
        self.quitButton = Button(self.frame, text = 'Quit', width = 25, command = self.close_windows)
        #self.quitButton.pack()
        self.frame.pack()
        filepath = askopenfilename(title="Ouvrir une image",filetypes=[('png files','.png'),('all files','.*')])
        file_nom = filepath.rsplit("/",1)[-1]
        new_name = file_nom.rsplit(".",1)[0] + "SLIC.ppm"
        new_name_comp = file_nom.rsplit(".",1)[0] + "Comp.ppm"
        new_name_palet = file_nom.rsplit(".",1)[0] + "Palet.ppm"
        new_name_index = file_nom.rsplit(".",1)[0] + "Index.pgm"
        f_p = "out/"
        f_p2 = f_p + file_nom.rsplit(".",1)[0] + "Index.pgm"
        new_path = filepath.rsplit(".",1)[0] + "Index.pgm"
        f_p3 = "./" + f_p2
        #new_path = f_p2
        f_p4 = f_p + file_nom.rsplit(".",1)[0] + "SLIC.ppm"
        print("new_path = " + new_path)
        print("f_p3 = " + f_p3)
        #f_p3 for comp et fp_4 for slic
        image2 = Image.open(f_p4)
        if image2.width>=1900 or image2.height>=1000 : resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
        else : resize_image2 = image2 
        #resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
        test2 = ImageTk.PhotoImage(resize_image2)

        label2 = Label(master,image=test2)
        label2.image = test2

        # Position image
        label2.place(x=0, y=0)
        if image2.width>=1900 or image2.height>=1000 :
           w = int(image2.width*0.5)
           h = int(image2.height*0.5)
        else :
           w = image2.width
           h = image2.height
        image3 = Image.open(f_p3)
        if image3.width>=1900 or image3.height>=1000 : resize_image3 = image3.resize((int(image3.width*0.5),int(image3.height*0.5)))
        else : resize_image3 = image3 
        #resize_image2 = image2.resize((int(image2.width*0.5),int(image2.height*0.5)))
        test3 = ImageTk.PhotoImage(resize_image3)

        label3 = Label(master,image=test3)
        label3.image = test3

        # Position image
        label3.place(x=0, y=0)
        if image3.width>=1900 or image3.height>=1000 :
           w1 = int(image3.width*0.5)
           h1 = int(image3.height*0.5)
        else :
           w1 = image3.width
           h1 = image3.height
        master.geometry(f"{w1}x{h1}")
    def close_windows(self):
        self.master.destroy()

#class Yep:
    #def __init__(self, master):
        #app = Demo1(master)
def Seg(k,m):
    Demo1(fenetre, k,m)
def Comp():
    Demo2(fenetre)

def main(): 
    #fenetre = Tk()
    k = StringVar();
    m = StringVar();
    w_K = Spinbox(fenetre, values=(200, 300, 400, 750, 1000), textvariable=k)
    w_K.pack()
    w_m = Spinbox(fenetre, values=(1, 3, 10, 15, 40), textvariable=m)
    w_m.pack()
    menubar = Menu(fenetre)

    menu1 = Menu(menubar, tearoff=0)
    menu1.add_command(label="Segmentation", command=lambda :Seg(k,m))
    menu1.add_command(label="Compression", command=Comp)
    #menu1.add_command(label="Compression", command=Demo2(fenetre))
    menu1.add_command(label="Quitter", command=fenetre.quit)
    menubar.add_cascade(label="Fichier", menu=menu1)

    fenetre.config(menu=menubar)

    
    #B = Button(fenetre,text = "HelloW",command = Yep(fenetre))
    #B.pack()
    #app = Demo1(fenetre)
    fenetre.mainloop()

if __name__ == '__main__':
    main()