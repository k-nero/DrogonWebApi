@echo off
SET PROTO_FILES_PATH=proto
SET OUT_DIR=proto
SET PROTOC_PATH=..\vcpkg_installed\x64-windows\tools\protobuf\protoc.exe
SET CPP_PLUG_INPATH=..\vcpkg_installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe

REM Compiling proto files
FOR %%G IN ("%PROTO_FILES_PATH%\*.proto") DO (
    echo Compiling %%G
    %PROTOC_PATH% -I=%PROTO_FILES_PATH% --cpp_out=%OUT_DIR% %%G
    %PROTOC_PATH% -I=%PROTO_FILES_PATH% --grpc_out=%OUT_DIR% --plugin=protoc-gen-grpc=%CPP_PLUG_INPATH% %%G
)

echo Compilation finished.
pause