# CoSIM-Gres (Complex Object representation and SIMilarity queries on Postgres)

CoSIM-Gres is a system to support the execution of similarity queries, taking advantage of Metric Access Methods. 

## Instructions to install
The commands must be executed on Fedora 40.

### Download and extract the project on "kiara" folder: /Documents/kiara

### Activate the root user
```su```

### Yum update and upgrade
```
yum update
yum upgrade
```

### Installing packages for Postgres compilation
```
yum install -y bison-devel
yum install -y readline-devel
yum install -y zlib-devel
yum install -y openssl-devel wget
yum groupinstall -y ‘Development Tools’
yum install bzip2
sudo dnf install gcc-c++
```

### Create postgres user
```
adduser postgres
```
### Download and compilation of Postgres 13.2
```
wget https://ftp.postgresql.org/pub/source/v13.2/postgresql-13.2.tar.bz2
tar -xvf postgresql-13.2.tar.bz2
cd postgresql-13.2
./configure
make
```

#### The expected output of make is: All of PostgreSQL successfully made. Ready to install.

```make install```
#### The expected output of 'make install' is: PostgreSQL installation complete.

### Creating the folder for Postgres data:
```
mkdir /usr/local/pgsql/data
chown postgres /usr/local/pgsql/data
```

### Change user: postgres
```
su - postgres
/usr/local/pgsql/bin/initdb -D /usr/local/pgsql/data
```

### Expected output: 'Success. You can now start the database server ...'

```
exit
```

### Change to ROOT user again
```
su
```

### ! Enter the kiara folder: /Documents/kiara
```
cd /Documents/kiara
cp contrib/start-scripts/linux /etc/init.d/postgresql
chmod 775 /etc/init.d/postgresql
chkconfig --add postgresql
echo 'PATH=$PATH:/usr/local/pgsql/bin/' >> /etc/profile
echo "export PATH" >> /etc/profile
ln -s /usr/local/pgsql/bin/* /usr/local/bin
```

### ! Go back to kiara folder and compile the Distance Functions
```
cd ..
cd DistanceFunctions
make
```
### Starting Postgres
```
service postgresql start
service postgresql status
```

### ! Change to the default user
```
exit
```
### Execute::
```
psql -c "CREATE USER $(whoami) SUPERUSER;" --username=postgres
psql -c "CREATE DATABASE $(whoami);" --username=postgres
```

### Enter the kiara folder 
```
cd /Documents/kiara
```

### Execute:
```
psql -f SimilarityQueries/schema.sql
psql -f DistanceFunctions/distance_functions.sql
psql -f SimilarityQueries/functions_bytea.sql
```

### Scripts for making available the Slim-tree MAM
```
sudo service postgresql stop
cd sims/lib
cd arboretumgcc
make
cd ../artemisgcc
make
```

### Going back to kiara directory
```
cd /Documents/kiara
```

### Compiling the Metric Access (MAM)
```
cd MetricAccess
sudo make
cd ..
```

### Restart Postgres
```
sudo service postgresql start
sudo service postgresql status
```

### Making available the MAM on Postgres
```psql -U postgres -f MetricAccess/MetricAccess.sql```

### Quit Postgres
```exit```

### Execute dummy tests
```psql -U postgres -f tests_dummy/script.sql```
