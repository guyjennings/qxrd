message("RPM Build Rules")

APPS = $$files(apps/*-app, false)

for(APP,APPS) {
  APP ~= s/apps.(.*)-app/\\1

  include(apps/$${APP}-app/$${APP}.version.pri)

  message($${APP} VERSION $${VERSION} $${DESCRIPTION})

  QMAKE_EXTRA_TARGETS += $${APP}-spec

  $${APP}-spec.depends += qxrd.spec

  $${APP}-spec.commands = \
    perl -p -pe '\'s/\\@VERSION\\@/$${VERSION}/g;\'' \
            -pe '\'s/\\@APP\\@/$${APP}/g;\'' \
            -pe '\'s/\\@DESC\\@/$${DESCRIPTION}/g;\'' \
            $${PWD}/app.spec.in >> qxrd.spec

  rpm.depends += $${APP}-spec
}

message("APPS = $$APPS")

QMAKE_EXTRA_TARGETS += rpm

include(qxrd.version.pri)
