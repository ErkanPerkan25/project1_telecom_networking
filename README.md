# project1_telecom_networking


## **Eric Hansson**

- Platform:<br>
Arch Linux

- Details to compile program:<br>
Created a Makefile which creates an executable called `webServer`<br>
To create the executable, you need to run this command:<br>
```
make
```

- Details to run program: 
To run the executable called `webServer` you are going to have to provide the port number that it will be running.
```
./webServer <port number>
```
Replace `<port number>` with the port number you want to run your web server from.

- Any known bugs?
There seems to be no bugs with the program. At leas of what I have noticed. <br>

- Overview of solving project:
For solving the project I took and made sure that I was doable to send little bit of data. And when that worked
I started to focus on how to read more data that the client sends to the web server (Web Browser). After I was
able to read all the data I created the respone back to the client if the file existed, and also if it did not 
exist.
