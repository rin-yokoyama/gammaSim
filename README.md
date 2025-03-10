# gammaSim
This is a gamma-ray detection simulation package using Geant4 for a coaxial HPGe detector.
## Requirement
- CMake >3.16
- C++17
- Geant4 version 11.2.1 (not checked with other versions)
- Apache Arrow
- Apache Parquet

In the case of Ubuntu, for example, install apache arrow and parquet as following.
```
sudo apt update
sudo apt install -y -V ca-certificates lsb-release wget
wget https://apache.jfrog.io/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb
sudo apt install -y -V ./apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb
sudo apt update
sudo apt install -y -V libarrow-dev
sudo apt install -y -V libparquet-dev
```
See "C++ and GLib (C) Packages" part in [https://arrow.apache.org/install/] for details.

## Compilation
source geant4.sh in your system before compilation. Then, 
```
mkdir build
cd build
cmake ..
make -j
```
If CMake couldn't find Geant4, try `cmake .. -DCMAKE_PREFIX_PATH=/path/to/geant4/lib/cmake/Geant4`

## How to run
```
build/exampleB1 run1.mac output_dir
```
To run the program in batch mode.
Just `build/exampleB1` to launch interpreter and viewer.

## Geometry settings
Variables for the geometry settings are defined in `include/ExpConstants.hh`.
You can change which source definition file to use in the simulation here.

## Gamma definitions
Text files in the `gamma_definitions` folder are the files defining the decay scheme and gamma emissions. The format of the file is as following.
```
[number of states]
[state name] [Beta-decay feeding ratio] [number of gamma-rays]
Egamma  to  probability p_gamma
[Egamma in keV] [state name to which the gamma decays] [branching ratio] [Gamma emission probability after the internal conversion correction]
...

[state name] [Beta-decay feeding ratio] [number of gamma-rays]
...
```

## Output
The program will create two folders under `output_dir` specified as an argument.
`detectorData` contains parquet files for energy deposit information and `particleData` contains information of emitted particles (particle type, energy, position, and direction). Each multi-threaded worker creates individual parquet file. You can read them by using pyspark or pandas.

The detectorData folder contains following data. The "eDep" (energy deposit) column is in MeV unit. Following pyspark command adds an "eDepkeV" column.
```
df = spark.read.parquet("file:///output_dir/detectorData")
df = df.withColumn("eDepkeV", F.col("eDep") * F.lit(1000))
df.show(10)
+--------+-------+-------+------+-------------------+------------------+
|workerId|eventId|detName|copyId|               eDep|           eDepkeV|
+--------+-------+-------+------+-------------------+------------------+
|       6|      0|     Ge|     0|                0.0|               0.0|
|       6|      1|     Ge|     0|0.19851344067257443|198.51344067257443|
|       6|      2|     Ge|     0|           1.173228|1173.2279999999998|
|       6|      3|     Ge|     0|                0.0|               0.0|
|       6|      4|     Ge|     0|                0.0|               0.0|
|       6|      5|     Ge|     0| 1.3324920000000005|1332.4920000000004|
|       6|      7|     Ge|     0|                0.0|               0.0|
|       6|      9|     Ge|     0| 0.4001991782363646|400.19917823636456|
|       6|     10|     Ge|     0| 0.9607120332095116| 960.7120332095116|
|       6|     11|     Ge|     0|  1.514555938793673|1514.5559387936728|
+--------+-------+-------+------+-------------------+------------------+
only showing top 10 rows
```

The `particleData` directory contains following data. This is an example data from 60Co point source at the center (x=0,y=0,z=0).
```
df2 = spark.read.parquet("file:///output_dir/particleData")
df2.show(10)
+--------+-------+-----+--------+------------------+--------------------+---+---+---+
|workerId|eventId|pName|  energy|             theta|                 phi|  x|  y|  z|
+--------+-------+-----+--------+------------------+--------------------+---+---+---+
|       3|      0|gamma|1173.228|1.7743586371677504|  1.5231992796101446|0.0|0.0|0.0|
|       3|      0|gamma|1332.492|1.0661990532587973| -0.0875328444408886|0.0|0.0|0.0|
|       3|      1|gamma|1173.228|1.7133523890191045|-0.16887838181554515|0.0|0.0|0.0|
|       3|      1|gamma|1332.492|2.0172931114813553|    2.36641835919301|0.0|0.0|0.0|
|       3|      2|gamma|1173.228|1.8598710446044515|   3.007934471475089|0.0|0.0|0.0|
|       3|      2|gamma|1332.492|0.7045927474017639| -3.0993805696586287|0.0|0.0|0.0|
|       3|      3|gamma|1173.228| 1.108905922936374|  2.1960027186156785|0.0|0.0|0.0|
|       3|      3|gamma|1332.492|1.2731026524154871| -1.1308933920535633|0.0|0.0|0.0|
|       3|      4|gamma|1173.228|1.5026636542160496|  -2.402032959034681|0.0|0.0|0.0|
|       3|      4|gamma|1332.492|1.3420243028825782| -1.6165292713005113|0.0|0.0|0.0|
+--------+-------+-----+--------+------------------+--------------------+---+---+---+
only showing top 10 rows
```

You can join the two dataframes by `workerId` and `eventId`
```
df = df.join(df2, ["workerId","eventId"], "left")
df.show(10)
+--------+-------+-------+------+------------------+------------------+-----+--------+------------------+--------------------+---+---+---+
|workerId|eventId|detName|copyId|              eDep|           eDepkeV|pName|  energy|             theta|                 phi|  x|  y|  z|
+--------+-------+-------+------+------------------+------------------+-----+--------+------------------+--------------------+---+---+---+
|       1|      1|     Ge|     0|               0.0|               0.0|gamma|1173.228|2.4996732126824353| 0.13288491512723882|0.0|0.0|0.0|
|       1|      1|     Ge|     0|               0.0|               0.0|gamma|1332.492| 1.988615915442027|  -2.077943103166752|0.0|0.0|0.0|
|       1|      4|     Ge|     0|1.1732280000000006|1173.2280000000005|gamma|1173.228|1.3006532843838798|  -2.334404594260986|0.0|0.0|0.0|
|       1|      4|     Ge|     0|1.1732280000000006|1173.2280000000005|gamma|1332.492|  1.70310571027622|  0.8631950087873395|0.0|0.0|0.0|
|       1|     12|     Ge|     0|2.1867721751751987|2186.7721751751988|gamma|1173.228|2.0462722209199096|-0.00359508025635...|0.0|0.0|0.0|
|       1|     12|     Ge|     0|2.1867721751751987|2186.7721751751988|gamma|1332.492|1.6458299770230969|  -2.123973683503156|0.0|0.0|0.0|
|       1|     14|     Ge|     0|1.3324920000000005|1332.4920000000004|gamma|1173.228| 2.563603740903674|  1.4533575828112488|0.0|0.0|0.0|
|       1|     14|     Ge|     0|1.3324920000000005|1332.4920000000004|gamma|1332.492| 1.145763463523689|  -2.304824712789032|0.0|0.0|0.0|
|       4|      6|     Ge|     0|          1.332492|          1332.492|gamma|1173.228|1.7508451195695751| -0.6120304978167037|0.0|0.0|0.0|
|       4|      6|     Ge|     0|          1.332492|          1332.492|gamma|1332.492| 2.400049085029184| -1.0808280316345389|0.0|0.0|0.0|
+--------+-------+-------+------+------------------+------------------+-----+--------+------------------+--------------------+---+---+---+
only showing top 10 rows
```
And then calculate the full energy efficiency as following.
```
en = 1173.228
count = df.filter(f"eDepkeV=={en} AND energy == {en}").count()
count/1000000
0.013038
```