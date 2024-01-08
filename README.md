# DrogonWebApp

### Dependency
Highly recommend using vcpkg to install these packages
- drogon: A C++14/17/20 based HTTP web application framework running on Linux/macOS/Unix/Windows
- inih: Simple .INI file parser in C, good for embedded systems
- SQLAPI : Can be found in lib folder
- boost/describe
- boost/json
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

