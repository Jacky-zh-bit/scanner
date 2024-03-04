import ctypes
import tkinter as tk

class Main_window():
    #主窗口
    def __init__(self):
        self.root=tk.Tk()
        #主窗口大小
        self.root.geometry('1200x600+100+100')
        self.root.title("SCANNER")
        
        #加入按钮
        self.buttons=Button_win(self.root)
        self.window_draw()
       

    def window_draw(self):
        self.root.mainloop()



class Text_square():
    #文本框

    def __init__(self,window):
        self.window=window;
        self.text_1=tk.Text(self.window,height=30,width=120)
        self.text_1.place(x=250,y=100)
        #滚动条
        self.scroll = tk.Scrollbar()
        self.scroll.pack(side=tk.RIGHT,fill=tk.Y)
 
        # 两个控件关联
        self.scroll.config(command=self.text_1.yview)
        self.text_1.config(yscrollcommand=self.scroll.set)



class Button_win():
    #按钮

    def __init__(self,window):
        self.window=window
        self.scanner=tk.Button(self.window,text='扫描文件夹',width=15,height=2,command=self.scanner_function).place(x=60,y=120)
        self.dir_info=tk.Button(self.window,text='文件夹信息浏览',width=15,height=2,command=self.dir_data_function).place(x=60,y=190)
        self.dir_fun=tk.Button(self.window,text='文件夹操作',width=15,height=2,command=self.dir_function).place(x=60,y=330)
        self.file_fun=tk.Button(self.window,text='文件操作',width=15,height=2,command=self.file_function).place(x=60,y=260)
        self.log_view=tk.Button(self.window,text='LOG浏览',width=15,height=2,command=self.log_function).place(x=60,y=400)

        #加入文本框
        self.text=Text_square(self.window)

    def scanner_function(self):
        #向c++文件传入01
        functions=ctypes.CDLL("./scanner.dll")
        functions.function(1)
        self.file_read("info.txt")

    def dir_data_function(self):
        #向c++文件传入02
        functions=ctypes.CDLL("./scanner.dll")
        functions.function(2)
        self.file_read("info.txt")

    def file_function(self):
        #向c++文件传入03
        functions=ctypes.CDLL("./scanner.dll")
        functions.function(3)
        self.file_read("info.txt")

    def dir_function(self):
        #向c++文件传入04
        functions=ctypes.CDLL("./scanner.dll")
        functions.function(4)
        self.file_read("info.txt")

    
        
    def log_function(self):
        self.file_read("myLOG.txt")


    def file_read(self,name):
        #文件读取
        #清空文本框
        self.text.text_1.delete("1.0", "end")
        with open(name, "r",encoding='gbk') as f:  
            data = f.read()  
            self.text.text_1.insert(1.0,data)


windows=Main_window()
