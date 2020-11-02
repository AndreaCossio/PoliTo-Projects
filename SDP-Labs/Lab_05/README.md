# Instructions

## Exercise 01

Compile, install and test the ```hello-5``` driver.

Write a report that lists the steps that you followed, and that indicates where you find the outputs.

## Exercise 02

- Compile the ```chardev_SDP_lab``` driver.
- Compile the program ```test_chardev.c```
- As *superuser* install ```chardev_SDP_lab``` driver.

After you have installed the module, look at the tail of the file ```/var/log/kern.log``` or use the command ```dmesg | tail```. The output will suggest you how to create the special file ```/dev/chardev_SDP_lab```.

Execute as *superuser* ```test_chardev /dev/chardev_SDP_lab```. Verify whether it works correctly or not.

Run ```echo something > /dev/chardev_SDP_lab```; try ```cat /dev/chardev_SDP_lab``` what is its output? Why ```test_chardev /dev/chardev_SDP_lab``` gives the correct results, whereas ```cat``` does not behave as expected?

Correct your device driver to produce the expected results using both ```test_chardev``` and ```cat```. In particular, verify that executing in sequence the commands:

- ```echo something > /dev/chardev_SDP_lab```
- ```cat /dev/chardev_SDP_lab```
- ```echo xxx > /dev/chardev_SDP_lab```
- ```cat /dev/chardev_SDP_lab```

you get the strings ```something``` and ```xxx```, respectively, nothing else.