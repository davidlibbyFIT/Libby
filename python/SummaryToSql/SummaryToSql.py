import os
import fnmatch
import csv
import re
import os.path

OutFile = "C:\Users\david.libby\Desktop\CheapSeats\outMeta.sql"
#OutFile = raw_input("Please enter Sql Out File Name: ")

#DirToSearch = "C:\Users\david.libby\Desktop\CheapSeats"
DirToSearch = "K:\Lab_Samples"
#DirToSearch = raw_input("Please enter Directory to search: ")
print "your Dir", DirToSearch

DBName = "LibbyTest"
#DBName = raw_input("Please enter MytSql Schema Name: ")

TableName = "LabResultsMetaDataIII"
#TableName = raw_input("Please enter MytSql Table Name: ")

CreateTableString = "CREATE TABLE `" + DBName + "`.`" + TableName + "`  (   `Filename` VARCHAR(512) NOT NULL COMMENT '',   `Mode` VARCHAR(45) NULL COMMENT '',   `Recalibrations` INT NULL COMMENT '',   `Stop Reason` VARCHAR(45) NULL COMMENT '',   `Fluid Volume Imaged` FLOAT NULL COMMENT '',   `Particle Count` INT NULL COMMENT '',   `Total` INT NULL COMMENT '',   `Used` INT NULL COMMENT '',   `Percentage Used` FLOAT NULL COMMENT '',   `Particles Per Used Image` FLOAT NULL COMMENT '',   `Frame Rate` FLOAT NULL COMMENT '',   `Intensity Mean` FLOAT NULL COMMENT '',   `Intensity Min` FLOAT NULL COMMENT '',   `Intensity Max` FLOAT NULL COMMENT '',   `Start` DATETIME NULL COMMENT '',   `End` DATETIME NULL COMMENT '',   `Sampling Time` VARCHAR(45) NULL COMMENT '',   `Software` VARCHAR(45) NULL COMMENT '',   `Magnification` VARCHAR(45) NULL COMMENT '',   `Calibration Factor` FLOAT NULL COMMENT '',   `SerialNo` VARCHAR(45) NULL COMMENT '',   `Number of Processors` INT NULL COMMENT '',   `Notes` MEDIUMTEXT NULL COMMENT '',   PRIMARY KEY (`Filename`)  COMMENT '',   UNIQUE INDEX `Filename_UNIQUE` (`Filename` ASC)  COMMENT '',`Priming Method` VARCHAR(45) NULL COMMENT '', `Flow Rate` VARCHAR(45) NULL COMMENT '',`Sample Volume Aspirated` FLOAT NULL COMMENT '',`Sample Volume Processed` FLOAT NULL COMMENT '',`Efficiency` FLOAT NULL COMMENT '', `DSP Firmware` VARCHAR(45) NULL COMMENT '', `Pump` VARCHAR(45) NULL COMMENT '', `Syringe Size` VARCHAR(45) NULL COMMENT '',`Distribution Board Firmware Version` VARCHAR(45) NULL COMMENT '');"

f = open(OutFile, 'w' )
f.write(  CreateTableString + '\n' )
f.close()


#function to find files in dir..
def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename

				
#Start Of Main Program
badcount = 0
Recordcount =0;
CurrentFileCount = 0
file_count = 0;
fileList = []

resetcount=0

print "Building List please wait"
for filename in find_files(DirToSearch, '*run_summary.txt'):
    fileList.append(filename)
    print filename
    resetcount +=1
    if resetcount > 100:
        resetcount=0
        print filename
    file_count += 1
    
#print "fileList =", fileList
#print "file_count =", file_count
#sys.exit()



for filename in fileList:
    #if summary is on the name then bail out..
    if "_summary.csv" in filename:
        print "Excluding " + filename        
        continue

    if not os.path.isfile(filename):
        continue
    
    #Open the File..
    tot_num_lines = open(filename, 'r').read().count("\n")
    resetcount=0;
    startsql = 'INSERT INTO `' + DBName + '`.`' + TableName + '` (`filename`,'

    cleanFileName = filename.replace("\\","/")
    cleanFileName = cleanFileName.replace ("'","")
    values = ' VALUES (\'' + cleanFileName + '\','

    resetcount=0;


    with open( filename, "rb" ) as theFile:
        linecount=0
        CurrentFileCount +=1
        #use CSV reader to process the file into key, value pairs based on the header and data.
        #reader = csv.DictReader( theFile )
        #print filename
        listOfCol=[]
        for line in theFile:
            line = line.strip()            
            if len(line) > 1 :
                TwoItems = line.split(":")
                #print line
                key=TwoItems[0].strip()
                if key in listOfCol:
                    continue
                data=TwoItems[1].strip()
                minute=""
                second=""
                if len(TwoItems)>2:
                    minute=TwoItems[2].strip()
                if len(TwoItems)>3:
                    second=TwoItems[3].strip()
                if len(data)>0:
                   data=data.replace(' ml/min','')
                   data=data.replace(' ml','')
                   data=data.replace(' fps','')
                   data=data.replace('%','')
                   if len(minute)>0:
                       data = data + ":" + minute
                   if len(second)>0:
                       data = data + ":" + second
                   #print "Key = " + key + ", Data = "+data
                   startsql += "`"+ key + "`,"
                   values += "'"+ data + "',"
                   listOfCol.append(key)
                   
                   

    notesName=filename.replace ('_run_summary','_notes')
    #print "Notes= "+notesName
    fullnote="";
    if os.path.isfile(notesName):
        with open( notesName, "rb" ) as NotesFile:
            for line in NotesFile:
                line = line.strip()
                line = line.replace("'",'')
                fullnote = fullnote + line +'\\n'        
            #print fullnote;
            startsql += "`"+ 'Notes' + "`,"
            values += "'"+ fullnote + "',"


    startsql = startsql.rstrip(',')
    values = values.rstrip(',')
    startsql += ')'
    values += ')'
    startsql += values + ';'

    f = open(OutFile, 'a' )
    f.write(  startsql + '\n' )
    #print startsql
    f.close()
    print "File = " ,CurrentFileCount," of ",file_count
                    

             
                   

#print "Bad COunt = " , badcount

