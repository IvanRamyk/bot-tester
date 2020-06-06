mkfifo pipeline && ./../../client/cmake-build-debug/testClient 8010 8011 < pipeline | ../team_1/cmake-build-debug/team_1 > pipeline
