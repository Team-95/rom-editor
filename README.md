Project setup

To build c++ addon


Run from addon folder

set HOME=C:\Dev\.electron-gyp 

node-gyp rebuild --target=<1.6.10 --arch=x64 --dist-url=https://atom.io/download/atom-shell


run npm install from root folder

in one terminal, run 'npm start'

in another terminal, run 'npm run electron:serve'
