CEG 433/633 P0 ReadMe.txt
prabhaker.mateti@wright.edu

This is start up code for the File Sys Project.  It is written in C++,
but using minimal features of OOP.  Good OOP style is not a goal in
CEG 433; e.g., our classes let almost all of their fields be public.

You are expected to *carefully* study the code in these files.  You
must understand their internals thoroughly.

The design follows faithfully the details of i-node based file
volumes, (to be) described in the lectures.  The code segments that
invoke TODO() are for you to (eventually) develop during the term.
The file fs33types.h has more detail than what is needed just for P0.
The extra declarations are useful in P1, P2, ...

We often do not use the typical constructors.  The methods named
create() and reCreate() are our "constructors."  The name create is
used when the simulated disk is "fresh" and a file volume is yet to be
made out of it.  The name reCreate is used when re-creating the
objects from the file volume image that is already present on the
simulated disk.

The P0 code as given is not robust.   You should check that the "new"
operator does return a non-0 pointer.  There are a few other checks
that I omitted in order to keep the main logic from being cluttered.

All methods, unless trivial, should have pre- and post-conditions.  In
the given code there are several examples of these.  You are expected
to supply all the missing pre- post- conditions!

-eof-
