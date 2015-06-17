import os
import fnmatch
import csv
import sqlite3

conn = sqlite3.connect('yes2.db')

conn.execute('''CREATE TABLE IF NOT EXISTS data (filename text)''')

# Save (commit) the changes
conn.commit()


def find_files(directory, pattern):
    for root, dirs, files in os.walk(directory):
        for basename in files:
            if fnmatch.fnmatch(basename, pattern):
                filename = os.path.join(root, basename)
                yield filename



try:
    print 1
    conn.execute('''ALTER TABLE data ADD COLUMN filename TEXT;''')
    print 1
    conn.execute('CREATE TABLE data ("Area (ABD)" REAL, "Aspect Ratio" REAL, "Average Blue" REAL, "Average Green" REAL, "Average Red" REAL, "BinHigh" INTEGER, "BinLow" INTEGER, "Calibration Factor" REAL, "Calibration Image" INTEGER, "Camera" INTEGER, "Capture X" INTEGER, "Capture Y" INTEGER, "Ch1 Area" REAL, "Ch1 Peak Area" REAL, "Ch1 Peak" REAL, "Ch1 Width" REAL, "Ch2 Area" REAL, "Ch2 Peak Area" REAL, "Ch2 Peak" REAL, "Ch2 Width" REAL, "Ch2/Ch1 Ratio" REAL, "Circle Fit" REAL, "Circularity (Hu)" REAL, "Class" TEXT, "Compactness" REAL, "Convex Perimeter" REAL, "Convexity" REAL, "Count" TEXT, "CumulativeFrequency" INTEGER, "CumulativePercentage" REAL, "Date" DATE, "Diameter (ABD)" REAL, "Diameter (ESD)" REAL, "Edge Gradient" REAL, "Elongation" REAL, "Feret Angle Max" REAL, "Feret Angle Min"  REAL, "Fiber Curl" REAL, "Fiber Straightness" REAL, "Filename" text, "Filter Score" TEXT, "Frequency" INTEGER, "Geodesic Aspect Ratio" REAL, "Geodesic Length" REAL, "Geodesic Thickness" REAL, "Image File" TEXT, "Image Height" INTEGER, "Image Width" INTEGER, "Image X" INTEGER, "Image Y" INTEGER, "Intensity" REAL, "Length" REAL, "List File" TEXT, "List Name" TEXT, "Particle ID" INTEGER, "Particles Per Chain" TEXT, "Perimeter" REAL, "Ratio Blue/Green" REAL, "Ratio Red/Blue" REAL, "Ratio Red/Green" REAL, "Roughness" REAL, "Scatter Area" REAL, "Scatter Peak Area" REAL, "Scatter Peak" REAL, "Scatter Width" REAL, "Sigma Intensity" REAL, "Source Image" INTEGER, "Sum Intensity" INTEGER, "Symmetry" REAL, "Time" TEXT, "Timestamp" TEXT, "Transparency" REAL, "Volume (ABD)" REAL, "Volume (ESD)" REAL, "Width" REAL);')
    print 2
except:
    A=1
    conn.close()
    stop()
conn.close()
exit()
badcount = 0
Recordcount =0;
filecount = 0;
for filename in find_files('C:\Users\david.libby\Desktop\sample Data', '*.csv'):
    with open( filename, "rb" ) as theFile:
        linecount=0
        filecount +=1
        reader = csv.DictReader( theFile )
        for line in reader:
            linecount +=1;
            #print 'Line=' ,line
            startsql = 'INSERT INTO data (\"filename\",'
            values = ' VALUES (\"' + filename + '\",'
            for k, m in line.items():
                #print k ,' = ', m
                #conn.execute('SELECT * FROM data ', k)
                #print c.fetchone()
                #k = k.replace("(","[")
                #k = k.replace(")","]")
                #k = k.replace(" ","_")
                try:
                    m = m.replace("\\","/")
                    k = '\"' + k + '\"'
                    m = '\"' + m + '\"'
                except:
                    badcount +=1
                    a=1;
                try:
                    conn.execute('''ALTER TABLE data ADD COLUMN ''' + k + ''' TEXT;''')
                except:
                    badcount +=1
                    A=1                
                if k is None:
                    badcount +=1
                    continue

                startsql += k + ","
                if m is None:
                    badcount +=1
                    continue

                values += m + ","

            try:
                startsql = startsql.rstrip(',')
                values = values.rstrip(',')
                startsql += ')'
                values += ')'
                startsql += values + ';'
                Recordcount +=1
                print "Filecount = " ,filecount,"Rec in file= ",linecount,"Overall Record Count=", Recordcount;
                conn.execute(startsql)
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

        conn.commit()
             
        #print 'Found C source:', filename
conn.close()
print "Bad COunt = " , badcount

