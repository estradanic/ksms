echo "Cleaning up previous installs..."
rm ~/.ksms/ksms.AppImage 2> /dev/null
rm ~/.ksms/ksms.png 2> /dev/null
rm ~/.local/share/applications/ksms.desktop
echo "done"

echo "Installing..."
mkdir ~/.ksms 2> /dev/null
cp ksms.AppImage ~/.ksms/ksms.AppImage
cp ksms.png ~/.ksms/ksms.png
echo "Icon=$HOME/.ksms/ksms.png" >> ksms.desktop
cp ksms.desktop ~/.local/share/applications/ksms.desktop
xdg-desktop-menu forceupdate
echo "done"

exit 0
