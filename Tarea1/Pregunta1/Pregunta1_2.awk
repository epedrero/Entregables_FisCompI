#!/usr/bin/awk -f

BEGIN {
	FS=" "
	v1=0
	v2=0
	n1=0
	n2=0
}
$2==1{
	v1+=sqrt($6^2+$7^2+$8^2)
	n1++
}
$2==2{
	v2+=sqrt($6^2+$7^2+$8^2)
	n2++	
}
END{
	print "Velocidad promedio Tipo 1:",v1/n1,"\nVelocidad promedio Tipo 2:", v2/n2,"\nVelocidad promedio total:",(v1+v2)/(n1+n2)
}