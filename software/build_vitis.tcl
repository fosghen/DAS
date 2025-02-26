set tclPath [pwd]
cd $tclPath
cd ..
set projpath ../../DAS_ps_from_git
puts "Current workspace is $projpath"
setws $projpath
getws
set xsaName DAS_200
set appName DAS_200_ps

#Create a new platform
platform create -name {$xsaName} -hw {../hardware/hw/$xsaName.xsa} -proc {ps7_cortexa9_0} -os {standalone} -out {$projpath};platform write
platform read {$projpath/$xsaName/platform.spr}
importprojects $projpath/$xsaName
platform active {$xsaName}

repo -add-platforms $xsaName
importsources -name $xsaName/zynq_fsbl -path $tclPath/src/fsbl -target-path ./
domain active
#Create a new application
app create -name $appName -platform $xsaName -domain standalone_domain -template "Empty Application"
importsources -name $appName -path $tclPath/src/$appName
bsp setlib -name lwip213 -ver 1.0
bsp config lwip_dhcp true
bsp config pbuf_pool_size 2048
bsp regenerate
#Build platform project
platform generate
#Build application project
append appName "_system"
sysproj build -name $appName






