README: NS2 ASSIGNMENT
This code was submitted as a part of ECEN 602 Assignment at Texas A&M University.
Developed by: Sathyajit Kulkarni and Satya Aditya Praneeth Emani

Architecture:
- Modular programming, includes well commented lines to describe functionality.

Usage:
ns filename.tcl <BUFFER MANAGEMENT TYPE> <Scenario Number>
Example usage: 
ns ns2.tcl DROPTAIL 1
ns ns2.tcl RED 2

Errata:
Giving improper inputs in the command line was throw console message on terminal.
After each case and its TCP type is implemented, messages are thrown on the terminal for user knowledge.

Sometimes, (don't know why), xgraph fails to perform graphing. This maybe due to some internal segmentation fault. Not a bug related to code.

To get outputs, go to folder and gedit output1.tr and gedit output2.tr.

To get graphs, go to folder and then xgraph output1.tr and xgraph output2.tr

Layout:
Custom layout was done so that the routers and sources and receivers could be custom made.
Simulation:
Simulation was run for 180ms and the throughput was put in the files output1.tr, output2.tr and output3.tr.

This code has been extensively tested. Please do not hesitate to contact me via email at sathyajit92@gmail.com.


Thanks!
