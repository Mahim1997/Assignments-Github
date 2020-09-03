clear
echo "PUSHING ALL TO ORIGIN"
git add --all
git commit -m $1
git push --all
echo "DONE!!"
clear
