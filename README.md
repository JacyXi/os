# os
For whom concerned about the project, you could do the following steps when you want to improve your part each day.

1. git fetch

   Never have you forgotten to fetch others' latest work from the remote repo.  In avoid of causing conflict, "git fecth" could make sure you keep up with.

2. git branch xxx
   git checkout xxx
   
   Your work should be being done in your personal branch to avoid mistakenly git push to remote repo. Also avoid that others fetch your unfinished work so that get confusion.

3. git add xxxxx.cpp

   "git add xxxxx.cpp" command could add your work on a temporary storage stage. You can "git add xxx" many times before "git commit -m "xxxx"" command.

4. git commit -m "xxxxx"

   This command will pack all of your git add items(actually called blobs) onto the local storage. You could see this by using "git log" option.

5. git checkout master

   Back to the master branch.

6. git pull

   Make sure the local master is latest as remote master.

7. git merge xxx(your branch name)

   Merge all branch files into master.

8. git push

   Send your work to remote repo. Congradulations you succeed! Have a nice day (maybe night hhh).  
