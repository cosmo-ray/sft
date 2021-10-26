# sft

## dependancies
- qt(4,5), qmake, ocaml

## compilling

- git submodule update --init
- cd tool; make -f toy2ass1.mk
- qmake

## lyr file

the file containing the lyrics, with sylabes separator.
`&` is use as sylabes separator

example:
```
&ku&ru&shi&ku &ta&t&te &ka&na&shi&ku &ta&t&te
&CO&UR&T &wo &na&ka &de&wa &he&i&ki &na&no
```

## using SFT

- first: load a lyr file, and a video, slow down the speed.
- left click to time sylabe.
- right click to rewind of 1s 
- `o` to retime a sylabe
- `p` to skip one
- mouse wheel can also be used to ship rewind sylabe.

## using ./toy2ass

```
./toy2ass file.frm 1000.0
```
