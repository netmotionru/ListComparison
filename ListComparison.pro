TEMPLATE = subdirs

SUBDIRS += \
	app \
	lib/ListComparison

app.depends = lib/ListComparison
