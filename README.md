# Web Server (Using HTTP protocol)
**Project 1 in Telecommunication and Networking**

The program is a Unix (Posix) socket based web server. The web server takes in a single
command line argument indicating the port number the server is to utilize.

Author: **Eric Hansson** <br>

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

To test the program you can use **telnet** or a 'well-behaved' web browser like Google Chrome or FireFox.

- Any known bugs? <br>
There seems to be no bugs with the program, if it runs on a well-behaved web browser. The only thing I know might be an issue is if the program
runs with a not-well-behaved web browser it will not work.<br>

- Overview of solving project: <br>
For solving the project I took and made sure that I was doable to send little bit of data. And when that worked
I started to focus on how to read more data that the client sends to the web server (Web Browser). After I was
able to read all the data I created the respone back to the client if the file existed, and also if it did not 
exist. And from there keep on going to make sure that the things being sent to client is correct.
