import os
import fnmatch
import csv


#OutFile = "C:\Users\david.libby\Desktop\CheapSeats\out.sql"
OutFile = raw_input("Please enter Sql Out File Name: ")

#DirToSearch = "C:\Users\david.libby\Desktop\CheapSeats"
DirToSearch = raw_input("Please enter Directory to search: ")
#print "your Dir", DirToSearch

#DBName = "LibbyTest"
DBName = raw_input("Please enter MytSql Schema Name: ")

#TableName = "Table2"
TableName = raw_input("Please enter MytSql Table Name: ")

CreateTableString = "CREATE TABLE `" + DBName + "`.`" + TableName + "` (   `Area (ABD)` float DEFAULT NULL,   `Area (Filled)` float DEFAULT NULL,   `Aspect Ratio` float DEFAULT NULL,   `Average Blue` float DEFAULT NULL,   `Average Green` float DEFAULT NULL,   `Average Red` float DEFAULT NULL,   `BinHigh` int(11) DEFAULT NULL,   `BinLow` int(11) DEFAULT NULL,   `Biovolume (Cylinder)` float DEFAULT NULL,   `Biovolume (P. Spheroid)` float DEFAULT NULL,   `Biovolume (Sphere)` float DEFAULT NULL,   `Calibration Factor` float DEFAULT NULL,   `Calibration Image` int(11) DEFAULT NULL,   `Camera` int(11) DEFAULT NULL,   `Capture X` int(11) DEFAULT NULL,   `Capture Y` int(11) DEFAULT NULL,   `Ch1 Area` float DEFAULT NULL,   `Ch1 Peak Area` float DEFAULT NULL,   `Ch1 Peak` float DEFAULT NULL,   `Ch1 Width` float DEFAULT NULL,   `Ch2 Area` float DEFAULT NULL,   `Ch2 Peak Area` float DEFAULT NULL,   `Ch2 Peak` float DEFAULT NULL,   `Ch2 Width` float DEFAULT NULL,   `Ch2/Ch1 Ratio` float DEFAULT NULL,   `Circle Fit` float DEFAULT NULL,   `Circularity (Hu)` float DEFAULT NULL,   `Circularity` float DEFAULT NULL,   `Class` longtext,   `Compactness` float DEFAULT NULL,   `Convex Perimeter` float DEFAULT NULL,   `Convexity` float DEFAULT NULL,   `Count` longtext,   `CumulativeFrequency` int(11) DEFAULT NULL,   `CumulativePercentage` float DEFAULT NULL,   `Date` date DEFAULT NULL,   `Diameter (ABD)` float DEFAULT NULL,   `Diameter (ESD)` float DEFAULT NULL,   `Edge Gradient` float DEFAULT NULL,   `Elapsed Time` float DEFAULT NULL,   `Elongation` float DEFAULT NULL,   `Feret Angle Max` float DEFAULT NULL,   `Feret Angle Min` float DEFAULT NULL,   `Fiber Curl` float DEFAULT NULL,   `Fiber Straightness` float DEFAULT NULL,   `Filename` varchar(1024) CHARACTER SET utf8 DEFAULT NULL,   `Filter Score` longtext,   `Frequency` int(11) DEFAULT NULL,   `Geodesic Aspect Ratio` float DEFAULT NULL,   `Geodesic Length` float DEFAULT NULL,   `Geodesic Thickness` float DEFAULT NULL,   `Image File` longtext,   `Image Height` int(11) DEFAULT NULL,   `Image Width` int(11) DEFAULT NULL,   `Image X` int(11) DEFAULT NULL,   `Image Y` int(11) DEFAULT NULL,   `Intensity` float DEFAULT NULL,   `Length` float DEFAULT NULL,   `List File` longtext,   `List Name` longtext,   `Particle ID` int(11) DEFAULT NULL,   `Particles Per Chain` longtext,   `Perimeter` float DEFAULT NULL,   `Ratio Blue/Green` float DEFAULT NULL,   `Ratio Red/Blue` float DEFAULT NULL,   `Ratio Red/Green` float DEFAULT NULL,   `Roughness` float DEFAULT NULL,   `Scatter Area` float DEFAULT NULL,   `Scatter Peak Area` float DEFAULT NULL,   `Scatter Peak` float DEFAULT NULL,   `Scatter Width` float DEFAULT NULL,   `Sigma Intensity` float DEFAULT NULL,   `Source Image` int(11) DEFAULT NULL,   `Sphere Complement` float DEFAULT NULL,   `Sphere Count` float DEFAULT NULL,   `Sphere Unknown` float DEFAULT NULL,   `Sphere Volume` float DEFAULT NULL,   `Sum Intensity` int(11) DEFAULT NULL,   `Symmetry` float DEFAULT NULL,   `Time` varchar(1024) CHARACTER SET utf8 DEFAULT NULL,   `Timestamp` longtext,   `Transparency` float DEFAULT NULL,   `Volume (ABD)` float DEFAULT NULL,   `Volume (ESD)` float DEFAULT NULL,   `Width` float DEFAULT NULL ) ENGINE=InnoDB DEFAULT CHARSET=latin1;"

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

print "Building List please wait"
for filename in find_files(DirToSearch, 'data_export.csv'):
    fileList.append(filename)
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
    
    #Open the File..
    tot_num_lines = open(filename, 'r').read().count("\n")
    resetcount=0;

    with open( filename, "rb" ) as theFile:
        linecount=0
        CurrentFileCount +=1
        #use CSV reader to process the file into key, value pairs based on the header and data.
        reader = csv.DictReader( theFile )
        for line in reader:
            linecount +=1;
            #print 'Line=' ,line
            #Start to build the sql Strings
            startsql = 'INSERT INTO `' + DBName + '`.`' + TableName + '` (`filename`,'
            values = ' VALUES (\'' + filename + '\','
            for k, m in line.items():
                #print k ,' = ', m
                #conn.execute('SELECT * FROM data ', k)
                #print c.fetchone()
                #k = k.replace("(","[")
                #k = k.replace(")","]")
                #k = k.replace(" ","_")
                #Escape out Quotes
                try:
                    m = m.replace("\\","/")
                    k = '`' + k + '`'
                    m = '\'' + m + '\''
                except:
                    badcount +=1
                    a=1;
                #Test to see if the column is in the table cruce but works.
                #try:
                #    conn.execute('''ALTER TABLE data ADD COLUMN ''' + k + ''' TEXT;''')
                #except:
                #    badcount +=1
                #    A=1                

		#insure our Key is valid
                if k is None:
                    badcount +=1
                    continue

		#add Key it to the SQL string.
                startsql += k + ","

                #insure Value is Valid
                if m is None:
                    badcount +=1
                    continue

                values += m + ","

	    #Add new Row to Sql DB
            try:
                startsql = startsql.rstrip(',')
                values = values.rstrip(',')
                startsql += ')'
                values += ')'
                startsql += values + ';'
                Recordcount +=1
                resetcount += 1
                #print "FN = ",filename," FC = " ,CurrentFileCount," of ",file_count," Rc = ",linecount,"ORC = ", Recordcount;
                if resetcount >= 1000:
                    print "File = " ,CurrentFileCount," of ",file_count," row count = ",linecount," of ",tot_num_lines," tot count = ", Recordcount;
                    resetcount=0;
                    
                f = open(OutFile, 'a' )
                f.write(  startsql + '\n' )
                f.close()                
                    
                #print startsql
            except:
                print
                print
                print "filename = " + filename
                print "linecount = " , linecount
                print
                print "==========================================="
                print "startsql = " + startsql
                print "==========================================="
                print "values = " + values
                print "==========================================="
                badcount +=1
                if k is not None:
                    print "k = " + k
                if m is not None:
                    print "m = " + m
                stop()
            #Commit the data
             

#print "Bad COunt = " , badcount

