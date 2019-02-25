set >/tmp/rhoexec.out 2>&1
echo "
new ret, \$stdout in {
  \$receive(\"event:\",\"$1\",*ret) |
  for (@[code] <- ret) {
    code |
     stdout!([\"event: triggered\",\"$1\"])
  }
}" >/tmp/$1.rho
./rhoexec /tmp/$1.rho
#rm /tmp/$1.rho
