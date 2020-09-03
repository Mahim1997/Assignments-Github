#!/bin/bash
clear


#----------------------------Global Variables Begin-----------------------------------
#CleanDirectory is the clean directory where we unzip all the submissions

tempDir="TemporaryDirectory"
doneZippedFolder="DoneZippedFolder"
correctZipFile="CorrectZip.txt"
allSubmissions="SubmissionsAll.zip"
cleanDirectory="CleanDirectory"
inputFileCSV="CSE_322.csv"
id_of_student="0"
tenMarks='TenMarks.txt'
fiveMarks='FiveMarks.txt'
zeroMarks='ZeroMarks.txt'
absentList='Absents.txt'
absentListTextFile='Absents.txt'
inputFileCSVName='CSE_322.csv'

columnOfIDInCSV=1			#In CSV File, which column gives the ID





#----------------------------Global Variables End-----------------------------------







#========================================Functions begin==============================================
createFoldersAndFiles(){
	
	rm -rf Output/Extra/
	rm -rf Output/

	mkdir Output
	mkdir Output/Extra


	touch $correctZipFile
	echo -n > $correctZipFile

	touch $tenMarks
	echo -n > $tenMarks

	touch $fiveMarks
	echo -n > $fiveMarks

	touch $zeroMarks
	echo -n > $zeroMarks

}

unzipAll(){
	rm -rf CleanDirectory/  		#if CleanDirectory exists , delete it..

	unzip "SubmissionsAll.zip" -d CleanDirectory 	#unzip everything

	mkdir CleanDirectory/DoneZippedFolder
}

findRollFromFile(){
	file="$1"
	id_of_student=`echo $file|tail -c 12|head -c 7`
}

checkFormat(){
	file="$1"
	if [[ $file = *_[0-9][0-9][0-9][0-9][0-9][0-9][0-9]_assignsubmission_file_[0-9][0-9]05[0-9][0-9][0-9].zip ]]; then
	#	echo "File <$file> format is correct return 1"
		return 1;
	else
	#	echo "Wrong file format <$file>"
		return 0;
	fi		
}

makeCorrectZipFile(){	#Form the AllCorrectZipFilesSubmit.txt
	file="$1"; newPath="$2"; inputFileCSV="$3"; doneZipFolder="$3"; 
	#echo "Inside makeCorrectZipFile ... , file is <$file> , newPath is <$newPath> and inputFileCSV is <$inputFileCSV> "

	error=0
	checkFormat "$file"
	res="$?"
	#echo "Inside makeCorrectZipFile() ... result is $res"
	if [[ res -eq 1 ]]; then
		#FORMAT CORRECT , extract roll number
		rollNum=`echo "$file" | tail -c 12 | head -c 7 `
		#echo "Roll is <$rollNum>"
		echo "$rollNum" >> "../$correctZipFile"
	else
		#WRONG FORMAT
		echo "WRONG FORMAT For file <$file>"
	fi
}

doesExistInCorrectZipFiles(){	#For some reason,  if  grep -q $1 $outputFile ; then .. doesnot work, so manual checking is done !!
     #echo "Inside doesExist function, arg is $1 and output file is $outputFile"
	correctFile="$2"
	 while IFS= read -r rollRead        #read until 
	 do
	      #echo "Roll being read is <$rollRead> and input roll is <$1>"
	      if [[ $rollRead = $1 ]]; then
	           #echo 'RETURN 1'
	           return 1;
	           #statements
	      fi  
	 done < "$correctFile"

	 #echo 'RETURN 0'
	 return 0;
}

createAbsentListInitial(){
	#Go to CleanDirectory/
	cd CleanDirectory/
	for file in *.zip
	do
		#echo "Checking for file = <$file>"
		if [[ $file != "*.zip" ]]; then
			if [[ $file != "$allSubmissions" ]]; then
				#echo "IF condition for file = <$file>"
				makeCorrectZipFile "$file" $tempDir/ $inputFileCSV $doneZipFolder
			fi
		fi

	done

	#Check with roster and output all wrong things in ../Output/Absents.txt
	absenteeList="../Output/Absents.txt"
	touch $absenteeList
	echo -n > $absenteeList		#Erase any content of Absentee.txt

	echo "We are inside path : $(pwd)"

	input="../$inputFileCSV"
	
	for x in `cut -d , -f $columnOfIDInCSV "$input"`
	do
		 
		if [[ ${#x} -ge 2 ]]; then		#Remove the " thing 
			#echo -n "Now is:<$x> "
			roll=`echo $x| head -c 7`
			#echo " and roll is: <$roll>"	#correctly roll numbers appear from csv file
			
			#Lets check if this roll exists in CorrectFiles.txt
			#echo "Checking for <$roll>"
			doesExistInCorrectZipFiles "$roll" "../$correctZipFile"

			if [[ $? -ne 1 ]]; then 
	#roll does not exist in Correct Submitted so, add this to Absentee.txt
				#echo "===>>>>>>>>Does not exist so add to absenteeList : $roll"
				echo $roll >> $absenteeList 
			fi

			#echo $roll >> $allStudentsFile			#push this roll to Absentee.txt
		fi
				
	done

}

errorProcedureMakeNewFolder(){
	file="$1"; roll="$2";
	echo; echo "===>>>Inside errorProcedureMakeNewFolder(),  To make new folder for file <$file> and roll <$roll>"

	echo "-->Now we are at $(pwd)" #at CleanDirectory/TemporaryDirectory/

	#Move one step up to go at CleanDirectory/
	cd ..
	#Rename the TemporaryDirectory/ to rollnumber Folder
	mv "$tempDir"/ "$roll"/

	#Move this folder to Output/ Directory which is one step further
	 
	mv -t '../Output/' "$roll/"

	#Let's move the zip file to CorrectZipFolder
	mv "$file" "$doneZippedFolder/"

	#Also give zero marks 
	echo "$roll" >> "../$zeroMarks"

	echo ; echo "EXITING errorProcedureMakeNewFolder()... still at path = $(pwd)"
	echo ; echo ;
}

correctFormatProcedure(){
	file="$1" ; error=0	
#at CleanDirectory/ should also end at CleanDirectory/
	echo; echo; echo "--->>>Inside correctFormatProcedure() .. file is <$file>, path is $(pwd)" 
	
	#Since correct format, roll will be present in .zip file
	id_of_student=`echo $file|tail -c 12|head -c 7`
	roll="$id_of_student"

	error=0

	#Check if more than one folder exists
	cd $tempDir/ 	#Move to CleanDirectory/TemporaryDirectory
	countOfFolders=0
	OLDIFS=$IFS
	IFS=$'\n'
	folderName="NULL_FOLDER"
	for dir in `find . -maxdepth 1 -type d `
	do
		#echo "->Dir is <$dir>"
		if [[ $dir != \. ]]; then
			#echo "======>>>>Directory is <$dir>"
			folderName=`echo $dir | tail -c $(( ${#dir} - 1 ))`
			countOfFolders=$((countOfFolders+1))
			
		fi
	done 
	IFS=$OLDIFS
#Still in CleanDirectory/TemporaryDirectory
	countOfFiles=0
	for fileIterator in `find -maxdepth 1 -type f -exec echo {} \;`
	do
		countOfFiles=$(( countOfFiles + 1 ))
	done

	if [[ $countOfFolders -gt 1 ]]; then
		error=1
	fi
	if [[ $countOfFiles -gt 0 ]]; then
		error=2
	fi


	if [[ $error -eq 0 ]]; then
#Still in CleanDirectory/TemporaryDirectory
		#no mistakes i.e. only one folder (Compare folder names)
		if [[ "$folderName" = "$roll" ]]; then
			#Full marks as roll and folder names match
			echo "$roll" >> "../../$tenMarks"

		elif [[ "$folderName" = *"$roll"* ]]; then
			#5 marks as folder name contains roll somewhere
			echo "$roll" >> "../../$fiveMarks"
			mv "$folderName/" "$roll/"	#Rename folder to its roll number (Found from zip file)
		else
			#zero marks as no matching
			echo "$roll" >> "../../$zeroMarks"
			mv "$folderName/" "$roll/"	#Rename folder to its roll number (Found from zip file)			
		fi
	#Finally, move folder to Output Folder (if renaming was necessary, we renamed the folder to output folder)
		mv -t "../../Output/" "$roll"
#Still in CleanDirectory/TemporaryDirectory
		cd ..
#Move up to now at CleanDirectory/
	else 
		#yes multiple files ... Still in CleanDirectory/TemporaryDirectory/
		
		echo "$roll" >> "../../$zeroMarks"  #Write this roll to ZeroMarks.txt

		cd ..  #move up , rename TemporaryDirectory with rollNumber
		mv "$tempDir/" "$roll/"
#Now we at CleanDirectory/ with a folder called $roll number , move this to ../Output folder
		mv -t "../Output/" "$roll"
	fi
 



#Move the zip file to OutputZip , still Now we at CleanDirectory/
	mv "$file" "$doneZippedFolder/"
	echo; echo; echo "EXITING correctFormatProcedure().. path is now $(pwd)"; echo; echo; #Should end up at CleanDirectory/
}



wrongFormatProcedure(){
	fileName="$1"
	echo "--->>>Inside wrongFormatProcedure() .. file is <$file>, path is $(pwd)"; echo ; echo 
#Now At CleanDirectory/

#Cant find roll number from zip ... Let's extract the name from zip file
	name=`echo $fileName| cut -d'_' -f 1`
	echo "Name of student is <$name>"

 #Let's check how many people have the same name, and extract the ID
 	
 	OLDIFS=$IFS
 	IFS=$'\n'
 	id="NULL_ID"
 	numPeople=0
 	for row in `grep -i "$name"  "../$inputFileCSVName"`
	do
		numPeople=$((numPeople+1))
		echo; echo "At line 293, Row is <$row>"
		id=`echo "$row"|cut -d',' -f $columnOfIDInCSV|tail -c 9|head -c 7` 	
		echo "At line 295, ID is <$id> IF YOU CAN't FIND ID, above line e -f 2 use korte hobe."
	done 
	echo "Finally after checking .csv file, id found is <$id>"
	
#-----------------------------------Checking for multiple folders/files begin-------------------------

	
#Lets go to tempDirectory
	echo "At line 302, tempDir/ is $tempDir/"
	cd $tempDir/
#At CleanDirectory/TemporaryDirectory now

	error=0
	countOfFiles=0
	for fileIterator in `find -maxdepth 1 -type f -exec echo {} \;`
	do
		countOfFiles=$(( countOfFiles + 1 ))
	done

	countOfFolders=0

	folderName="NULL_FOLDER"
	echo "At line 316 ... checking ... path is $(pwd)"
	for dir in `find . -maxdepth 1 -type d `
	do
		echo "->Dir is <$dir>"
		if [[ $dir != \. ]]; then
			echo "======>>>>Directory is <$dir>"
			folderName=`echo $dir | tail -c $(( ${#dir} - 1 ))`
			countOfFolders=$((countOfFolders+1))
			
		fi
	done 

	if [[ $countOfFiles -gt 0 ]]; then
		error=2
	fi
	if [[ $countOfFolders -gt 1 ]]; then
		error=3
	fi
	if [[ $error -eq 0 ]]; then
		echo ; echo "===>>Line 332, no errors"
	else
		echo; echo "--->>>Line 334, multiple file/folders exist error"
	fi

#Still At CleanDirectory/TemporaryDirectory
#-----------------------------------Checking for multiple folders/files done---------------


#-------------------------------------------Get new folder name begin-----------------------------------------
	newFolderName=$name 	#By default name dhore rakhi
	toPutFolderName="Output/" 	#By default Output Folder
	#id=""
	probInResolve=0
	if [[ $numPeople -eq 0 ]]; then
		newFolderName=$name 		#ID With this name, not found in roster, just use name, can't resolve, move to Extra
		probInResolve=1
		echo ; echo "-->At line 348, can't resolve for name: <$name>"
	elif [[ $numPeople -eq 1 ]]; then
#only one person found, CAN resolve, need to remove from Absentee List still at CleanDirectory/TemporaryDirectory now
		newFolderName="$id" #we have rename to ID(found) 
		echo ; echo "-->At line 352, we can resolve for name: <$name>, id = <$id> , absenteeList thika deleted.."
		#sed "/$rollNum/D" ZeroMarks.txt 	#Delete works
		sed -i "/$id/D" "../../Output/Absents.txt"	#Remove this ID From Absents.txt	
		echo "$id" >> "../../$zeroMarks" #give zero marks to this ID
		rollNum="$id"
		probInResolve=0
	else
		#Multiple people found, TO DO ...
		cntOfAbsents=0; id="NULL_ID";
		for row in `grep -i "$name"  "$inputFileCSVName"`
		do
			numPeople=$((numPeople+1))
			id=`echo "$row"|cut -d',' -f $columnOfIDInCSV|tail -c 9|head -c 7` 
			 
			if grep -q "$id" "$file"; then
		  		cntOfAbsents=$(( cntOfAbsents + 1 ))
			fi

		done 
		if [[ $cntOfAbsents -eq 1 ]]; then
			#IF Roll ta Absents.txt teh aase (ektai roll) so , CAN RESOLVE...
			probInResolve=0
			rollNum="$id";
			newFolderName="$id";	#We shall now rename folder with filID
		else
			#ELSE , Roll ta Absents.txt teh nai othoba more than one ase , so can't resolve
			newFolderName="$name" 	#Otherwise keep the name ...
			probInResolve=1
		fi

		#ID Retreive kora eikhanei uchit ...
	fi

#-------------------------------------------Get new folder name end-----------------------------------------


#Still At CleanDirectory/TemporaryDirectory now
	#Folder checking
	if [[ $error -eq 0 ]]; then 			#only one folder exists...
		echo ; echo "At line 373, only one folder exists\
			, folder name is <$folderName> and probInResolve: <$probInResolve> and newFolderName is <$newFolderName>"
		echo "Path is $(pwd)"
		if [[ $probInResolve -eq 0 ]]; then #no problem in resolving, ID is retrieved in $rollNum
			#Still At CleanDirectory/TemporaryDirectory now
			if [[ "$folderName" != "$newFolderName" ]]; then #rename as mv oldName newName
				mv "$folderName" "$newFolderName" 	#rename the folder with ID(resolved)
			fi
			mv -t '../../Output/' "$newFolderName/"	#move this folder to Output
		fi
		if [[ $probInResolve -eq 1 ]]; then 	#can't reseolve, so rename with <name>
			if [[ "$folderName" != "$newFolderName" ]]; then #rename as mv oldName newName
				mv "$folderName" "$newFolderName" 	#rename the folder with name(unresolved)
				echo ; echo "==>>We need to move $newFolderName to ../../Output/Extra"
				mv -t '../../Output/Extra/' "$newFolderName/"	#move this folder to Output/Extra
			fi	
		fi
		
#NOW At CleanDirectory/
		cd ..
		mv "$file" "$doneZippedFolder" 	#move the zip file to DoneZipFolder
	else
		#multiple files/folders...

		echo "Multiple files/folders handling baaki"
#Still At CleanDirectory/TemporaryDirectory now
		cd .. 
#Move upto CleanDirectory/ now
		if [[ "$tempDir" != "$newFolderName" ]]; then #rename the TemporaryDirectory with $newFolderName
			mv "$tempDir" "$newFolderName" 	
		fi	
		#Now Move the $newFolderName to Output or Output/Extra
		if [[ $problemInResolve -ne 0 ]]; then 	#Unable to resolve for ID
			mv -t '../Output/Extra/' "$newFolderName/"	#move this folder to Output/Extra
		else
			mv -t '../Output/' "$newFolderName/"	#move this folder to Output/ because we were able to resolve 
		fi
		mv "$file" "$doneZippedFolder" 	#move the zip file to DoneZipFolder
	fi

	IFS=$OLDIFS

 	echo; echo; echo "EXITING wrongFormatProcedure() ... path is $(pwd)"; echo; echo  #Need to be at CleanDirectory/
}


unzipEachAndCheck(){
	clear
	echo "Inside unzipEachAndCheck() , pwd : $(pwd)"
	#cd CleanDirectory/ #Move into clean directory
	
	inputFileCSV="../../CSE_322.csv"
	doneZipFolder="DoneZippedFolder/"

	for file in *.zip
	do
		#Assumes that we are in CleanDirectory
		echo; echo "For loop of unzipEachAndCheck() .. BEFORE ... inside big unzipEachAndCheck() .. path is $(pwd)"; echo 
		rm -rf $tempDir/ 		#Delete the temporary folder(if exists) first
		mkdir $tempDir

		unzip "$file" -d $tempDir 	#Unzipping into temporary directory
		#clear
		checkFormat "$file"
		result=$?

		if [[ result -eq 1 ]]; then
			#Correct Format
			correctFormatProcedure "$file" 
			 
		else 
			#Wrong format
			wrongFormatProcedure "$file"
		fi


	done 
	echo "==>>Line 324, Inside unzipEachAndCheck() AFTER LOOP.. pwd is $(pwd) removing CleanDirectory/TemporaryDirectory"
	rm -rf "CleanDirectory/$tempDir/" 
}

writeMarks(){
	#First write marks of 10 to Output/Marks.txt , we are at directory where Output/Marks.txt and other marks are present
 	marksFile="$1"
 	filename="$2"
 	marks="$3"
 	echo "Now printing $marks marks students "
	while read -r id; do 	#-r to include escape characters as well
	    echo "ID: <$id>"
	    echo "$id $marks" >> "$marksFile"
		#Write the roll to Output/Marks.txt
	done < "$filename"


}

finalSteps(){
	marksFile="Output/Marks.txt"
	absentFile="Output/Absents.txt"
	writeMarks "$marksFile" "$tenMarks" "10"
	writeMarks "$marksFile" "$fiveMarks" "5"
	writeMarks "$marksFile" "$zeroMarks" "0"

#Sort Absentees.txt wrt Student ID
	sort -n "$absentFile" -o "$absentFile"

#Sort Marks File wrt Student ID
	#cat "$marksFile"
	echo "After sorting wrt IDs "
	sort -n -k 1 "$marksFile" -o "$marksFile"
	#cat "$marksFile"

#Move the zip file to a backup folder (newly created)
	rm -rf 'BackupFolder/'
	mkdir 'BackupFolder'
	mv 'SubmissionsAll.zip' 'BackupFolder/'  
	rm -rf 'CleanDirectory/'
	rm "$tenMarks"
	rm "$fiveMarks"
	rm "$zeroMarks"
	rm "$correctZipFile"
#Now lets delete things..

}

main(){
	echo "First we unzip all .."
	unzipAll
	clear
	echo "Unzip All is done .."
	createFoldersAndFiles
	echo "Create Folders is done "
	
	echo "Now create initial Absentee list .. "
	createAbsentListInitial
	echo "Creating initial Absentee list is done.."

	echo "Unzipping each and checking begins ... "
	unzipEachAndCheck
	echo "Unzipping each and checking ends ... "

	clear 
	echo "Now we will convert marks , we at path = $(pwd)" #CleanDirectory/
	cd .. 	#Move up

	finalSteps
}


#========================================Functions end=================================================














#=======================================<<<<<<<<<<<Actions begin>>>>>>>>>>======================================


main







#=======================================<<<<<<<<<<<Actions end>>>>>>>>>>======================================







