TEMPLATE = subdirs

!android {
SUBDIRS = \
	qmidiplayer-desktop \
	qmidiplayer-lite \
	sample-plugin \
	visualization \
        midifmt-plugin \
        bongocat-plugin
}
android {
SUBDIRS = \
	qmidiplayer-lite
}
