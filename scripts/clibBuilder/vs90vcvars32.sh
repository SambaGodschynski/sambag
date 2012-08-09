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
  if [ $(echo $1 | grep 'PATH') ] #grep 'INCLUDE\|LIB\|LIBPATH') ]
  then
       echo 1
       return
  fi
  echo 0
}

doexport()
{
 echo export $1=$2
 export $1=$2
}
_export()
{
  if [ $(check_path_conv $1) -eq 1 ]
  then
       x=$(conv_path $1)
       x=$(echo $x | sed 's/;/:/g' )
       NOCONV=1
  fi
  x=$(echo $x | sed 's/@set *//i')
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
      #if ! [ -z $NOCONV ]
      #then
         #x=$(echo $x | sed 's/;://g')
      #fi
  fi
  name=$(echo $x | grep -o '.*=' | sed 's/\=//')
  cnt=$(echo $x | grep -o '=.*' | sed 's/\=//')
  doexport $name $cnt
}

get_sdk_location()
{
    res=$(reg query "HKLM\SOFTWARE\Microsoft\Microsoft SDKs\Windows" /v "CurrentInstallFolder" | grep -io "[a-zA-Z]:.*$")
    if [ -z $1 ]
    then
        res= conv_path $res
    fi
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
#doexport VS90COMNTOOLS $SRC
exec_origbat $SRC/vsvars32.bat
doexport WindowsSdkDir "$(get_sdk_location)"

#by hand
SDK_LOCATION_UNCONV=$(get_sdk_location NOCONV)
doexport PATH $WindowsSdkDir\bin:$PATH
doexport INCLUDE $SDK_LOCATION_UNCONV\include\;$INCLUDE
doexport LIB $SDK_LOCATION_UNCONV\lib\;$LIB