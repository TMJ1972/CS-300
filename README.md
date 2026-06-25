CS 300: Data Structures and Algorithms

This repo holds my portfolio work from CS 300. There are two main pieces here: my Project One analysis of the data structures and my Project Two code that sorts and prints the course list.

What's in here


Project One: my run-time and memory analysis comparing the data structures I looked at for the advising program.
Project Two: the C++ program that loads the course data and prints all the courses in alphanumeric order.


Reflection

What was the problem you were solving in the projects for this course?

The whole thing was building an advising program for the ABCU Computer Science department. They needed something that could read a file of courses, let an advisor pull up a single course and its prerequisites, and print out the full course list in order. So the real problem was figuring out the best way to store all that course data in memory, then writing a program around it that actually does those things.

How did you approach the problem? Consider why data structures are important to understand.

I started with Project One, where I compared a vector, a hash table, and a binary search tree. For each one I figured out the Big O for loading, searching, and printing, and I looked at how much memory each would use. Doing that made it really clear why data structures matter. The same job can be fast or slow depending only on how you store the data. Once I had the analysis done I felt a lot more confident picking one, so for Project Two I went with a hash table using separate chaining.

How did you overcome any roadblocks you encountered while going through the activities or project?

Most of my problems were in the small details, not the big picture. Reading the file in correctly, making sure every prerequisite actually pointed to a real course, and dealing with collisions in the hash table all took me a few tries. What helped the most was using the debugger in Visual Studio 2022 to step through the code line by line and see what the data looked like as it loaded. I also tested small parts on their own before hooking everything together, which made it way easier to find where things were breaking.

How has your work on this project expanded your approach to designing software and developing programs?

This course got me to actually plan before jumping into code. Spending Project One on analysis first and then building in Project Two showed me how much easier things go when you pick your structure on purpose instead of just grabbing whatever you have used before. Now I try to look at what operations a program needs and what each one costs before I start writing.

How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

I tried to keep things broken into separate functions, like one for loading, one for searching, one for printing, and one for the menu, so each part is easy to read and change on its own. I also used clear names and wrote comments that explain why I did something instead of just repeating what the code says. Because I kept the data structure stuff separate from the menu and printing, it would be a lot easier to come back later and swap in a different structure without rewriting everything. Writing code that someone else, or honestly future me, can actually follow is something I pay attention to now.
