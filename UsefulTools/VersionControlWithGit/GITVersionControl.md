# Version Control with GIT
<font size =4> [Jon Leoliger](https://www.oreilly.com/library/view/version-control-with/9781449345037/) <br>


## Git quick reference
**git init** breate a new local repository<br>
**git add** add a new file to index<br>
**git commit** commit to local repository,with an option -m "a readme generated" , and -a is a useful option <br>
**My Email** git config --global user.email "chenht1996@gmail.com"<br>
**My username** git config --global user.name "Haitau1996"<br>
**git log** log files of recent commits，a very useful option is --pretty=oneline.<br>
**git reset** go back to the commit before, an option is needed to ratify which commit <br>
* --hard HEAD^ 
* --hard HEAD~100
* --hard SVN number<br>

**git reflog** to list the history<br>
**git status** to list new or modified files not yet commited<br>
**ssh -T git@github.com** connect to github<br>
**git branch** git push --set-upstream origin \<branch name> <br>
![avatar](figure/gitbranch.png)<br>
**tag** _git tag v1.0_  then we can use git tag to see the given tag.git tag v0.9 f52c633 gives tag for a pervious version. _git show \<tagname>_ shows the datailed information of specific version.<br>
**git branch** remote edit <br>
git branch -r -d origin/OffLin 删除了本地关于这个分支的track<br>
git push origin :OffLin 将本地删除的内容推送到远程的服务<br>
