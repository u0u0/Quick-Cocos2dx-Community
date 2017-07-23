#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BASENAME=`basename "$DIR"`

QUICK_V3_ROOT="$DIR"

echo ""
echo "QUICK_V3_ROOT = \"$QUICK_V3_ROOT\""
echo ""

# set Xcode
IDEApplicationwideBuildSettings=`defaults read com.apple.dt.Xcode IDEApplicationwideBuildSettings`
if [ -z "$IDEApplicationwideBuildSettings" ]; then
	defaults write com.apple.dt.Xcode IDEApplicationwideBuildSettings -dict
fi
defaults write com.apple.dt.Xcode IDEApplicationwideBuildSettings -dict-add QUICK_V3_ROOT "$QUICK_V3_ROOT"

IDESourceTreeDisplayNames=`defaults read com.apple.dt.Xcode IDESourceTreeDisplayNames`
if [ -z "$IDESourceTreeDisplayNames" ]; then
	defaults write com.apple.dt.Xcode IDESourceTreeDisplayNames -dict
fi
defaults write com.apple.dt.Xcode IDESourceTreeDisplayNames -dict-add QUICK_V3_ROOT QUICK_V3_ROOT

echo "> Xcode settings updated."

# set quick player
defaults write org.cocos.quick.player QUICK_V3_ROOT "$QUICK_V3_ROOT"
echo "> Quick player settings updated."

# set .bash_profile or .profile
echo "> Setup evn for shell:"$SHELL
ENVSTR="export QUICK_V3_ROOT=\`cat ~/.QUICK_V3_ROOT\`"
if [ "$SHELL" == "/bin/bash" ]; then
	PROFILE_NAME=~/.bash_profile
elif [ "$SHELL" == "/bin/sh" ]; then
	PROFILE_NAME=~/.profile
elif [ "$SHELL" == "/bin/zsh" ]; then
	PROFILE_NAME=~/.zshrc
elif [ "$SHELL" == "/bin/csh" ]; then
	PROFILE_NAME=~/.cshrc
	ENVSTR="set QUICK_V3_ROOT=\`cat ~/.QUICK_V3_ROOT\`"
else
	echo "Error, unknow shell!"
	exit -1
fi

sed -e '/QUICK_V3_ROOT/d' $PROFILE_NAME | sed -e '/add by quick-cocos2d-x setup/d' > $PROFILE_NAME.tmp

DATE=`date "+DATE: %Y-%m-%d TIME: %H:%M:%S"`
echo "# add by quick-cocos2d-x setup, $DATE" >> $PROFILE_NAME.tmp
echo $ENVSTR >> $PROFILE_NAME.tmp

DATE=`date "+%Y-%m-%d-%H%M%S"`
# cp $PROFILE_NAME $PROFILE_NAME-$DATE.bak
cp $PROFILE_NAME.tmp $PROFILE_NAME
rm $PROFILE_NAME.tmp

echo "> $PROFILE_NAME updated."

echo "$QUICK_V3_ROOT" > ~/.QUICK_V3_ROOT
echo "> ~/.QUICK_V3_ROOT updated."
echo ""

# Create link for player at QuickRoot
sudo cp -Rf $QUICK_V3_ROOT"/quick/player/player3.app" /Applications/
# add x permission
sudo chmod -R 755 $QUICK_V3_ROOT"/quick/player/player3.app"
sudo chmod -R 755 /Applications/player3.app
echo "Player3 has installed in /Applications"

echo ""
echo ""

echo "done. Please restart shell to make QUICK_V3_ROOT work"
echo ""
