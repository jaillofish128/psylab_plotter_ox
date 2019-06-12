# psylab_plotter_ox
The Psylab BloodyOx Plotter is a program dedicated to analyze and visualize biosignals collected oximetry stored by the Xenon system (K&Y Lab, National Yang-Ming-University, Taiwan). It was develped by me, Jaillofish128, in 2016 as a stand-in replacement for Xenon system's web interface, though over time it become a much more complex application.


This program is built on top of Qt 5 and qcustomplot (https://www.qcustomplot.com). It provides several useful functions for oximetery visualization and interpretation, some of which the web interface did not provide at the time: 


．Load data from the server or from local file and plot


．Notify when the requested data does not exist on the server


．Generate a histogram of oxygenenation


．Only plot data in given a time span


．Mark desaturation events and time length 


．Plot each line (finger activity, oxygenation & hear rate) be a given sample rate, which can make the plot more readable.


．Calculate the total time when blood oxygen level is below a given threshold. 


．(To some degree) recognize time spans when the wearer is not sleeping and skip them when doing marking and calculation mentioned above. 


．Filter out duplicated data and white noises of the oximeter

This program can be built with QtCreator, MSVC 2017 or CMake. Once the program is built, run the "Bloody_Ox" executable.
  
NOTE:Due to the internal structure change of Xenon system, oximetry records has a different format and new record are no longer passed to the old servers. As such, this program is deprecated and the source is kept for referencing purposes.
