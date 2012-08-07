#!/bin/bash
IFS=$'\n' #The variable IFS (or inter-field separator) in BASH determines what
          #characters act as delimiters. By default space, tab and newline
          #characters are used as delimiters. The variable can also be used
          #to define custom delimiters like comma, colon etc..


#shell version of vcvars32.bat
#converts all @SET xy=z commands to export xy=z
#tip: export works only with a 'source vcvars32.sh' invoke (not sh or bash)


conv_path() #converts windows path to cygwin path
{
  echo $1 | sed 's/\\/\//g' | sed 's/\([a-zA-Z]\):/\/cygdrive\/\1/g'
}

#chekcs whether a path has to be conv. or not.
check_path_conv()
{
  if [ $(echo $1 | grep 'INCLUDE\|LIB\|LIBPATH') ]
  then
       echo 0
       return
  fi
  echo 1
}

_export()
{
  if [ $(check_path_conv $1) -eq 1 ]
  then
       x=$(conv_path $1)
  fi
  x=$(echo $x | sed 's/@set *//i' | sed 's/;/:/g')
  if [ $(echo $x | grep '%.*%') ] #if %PATH%: append content of $PATH
  then
      #get varname
      varname=$(echo $x | grep -o '%.*%' | sed 's/%//g')
      #remove %PATH%
      x=$(echo $x | sed 's/%.*%//g')
      #append varcontent
      eval y=$(echo '$'$(echo $varname))
      x=$x:$y
      x=$(echo $x | sed 's/::/:/g')
      x=$(echo $x | sed 's/:$//g')
  fi
  name=$(echo $x | grep -o '.*=' | sed 's/\=//')
  cnt=$(echo $x | grep -o '=.*' | sed 's/\=//')
  echo export $name=$cnt
  export $name=$cnt
}

get_sdk_location()
{
    res=$(reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder" | grep -io "[a-zA-Z]:.*$")
    res= conv_path $res
    echo $res
}

exec_origbat()
{
    for x in $(cat $1 | grep -io '@SET.*$')
    do
      _export $x
    done
}

SRC=$(conv_path $VS90COMNTOOLS)
echo export VS90COMNTOOLS=$SRC
export VS90COMNTOOLS=$SRC
exec_origbat $SRC/vsvars32.bat
export WindowsSdkDir="$(get_sdk_location)"
echo export WindowsSdkDir="$(get_sdk_location)"
