# DrogonWebApp
[![Build Test](https://github.com/k-nero/SWD-Laundry-Backend/actions/workflows/aws.yml/badge.svg)](https://github.com/k-nero/SWD-Laundry-Backend/actions/workflows/aws.yml)
### Installation

Install Dependencies with vcpkg.

In case you don't have vcpkg installed, you can install it from [here]( https://github.com/microsoft/vcpkg) and add vcpkg to your PATH.
Then navigate to the root directory and run the following command to install the dependencies.
```sh 
$ vcpkg x-update-baseline
$ vcpkg install
```

Note: If you are using Visual Studio or MSBuild, please skip this step and enable the vcpkg integration in your IDE. Learn more about manifest mode [here]( https://learn.microsoft.com/en-us/vcpkg/consume/manifest-mode?tabs=msbuild%2Cbuild-MSBuild )

This project is using SQLAPI++ for database connection, you can download it from [here](https://www.sqlapi.com/Download/). After downloading, extract the files and copy the include and lib folders to the root directory of the project and copy the dll files to the build output directory.

  


### Database
- Currently using MSSQL, the connection param can be found in config.ini
### Build
- Create file config.ini in DrogonWebApp/DrogonServer 
```ini
[database]
server = (localdb)\MSSQLLocalDB@Drogon
username = sa
password = 12345
[bcrypt]
secret = secret
```
- Create private_key.pem and public_key.pem in DrogonWebApp/DrogonServer for JWT 
- Create private.key and certificate.crt in DrogonWebApp/DrogonServer to use Https

