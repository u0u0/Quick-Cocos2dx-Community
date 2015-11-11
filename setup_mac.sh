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
echo "> quick player settings updated."

# set .bash_profile or .profile
if [ -f ~/.bash_profile ]; then
	PROFILE_NAME=~/.bash_profile
else
	PROFILE_NAME=~/.profile
fi

sed -e '/QUICK_V3_ROOT/d' $PROFILE_NAME | sed -e '/add by quick-cocos2d-x setup/d' > $PROFILE_NAME.tmp

DATE=`date "+DATE: %Y-%m-%d TIME: %H:%M:%S"`
echo "# add by quick-cocos2d-x setup, $DATE" >> $PROFILE_NAME.tmp
echo "export QUICK_V3_ROOT=\`cat ~/.QUICK_V3_ROOT\`" >> $PROFILE_NAME.tmp

DATE=`date "+%Y-%m-%d-%H%M%S"`
# cp $PROFILE_NAME $PROFILE_NAME-$DATE.bak
cp $PROFILE_NAME.tmp $PROFILE_NAME
rm $PROFILE_NAME.tmp

echo "> $PROFILE_NAME updated."

echo "$QUICK_V3_ROOT" > ~/.QUICK_V3_ROOT
echo "> ~/.QUICK_V3_ROOT updated."
echo ""

export QUICK_V3_ROOT=`cat ~/.QUICK_V3_ROOT`

# Create link for player at QuickRoot
sudo cp -Rf $QUICK_V3_ROOT"/quick/player/player3.app" /Applications/
# add x permission
sudo chmod -R 755 $QUICK_V3_ROOT"/quick/player/player3.app"
sudo chmod -R 755 /Applications/player3.app
echo "Player3 has installed in /Applications"

echo ""
echo ""

echo "done."
echo ""
