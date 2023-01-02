void setBuildStatus(String message, String state) {
  step([
      $class: "GitHubCommitStatusSetter",
      reposSource: [$class: "ManuallyEnteredRepositorySource", url: "$env.GIT_URL}"],
      contextSource: [$class: "ManuallyEnteredCommitContextSource", context: "ci/jenkins/build-status"],
      errorHandlers: [[$class: "ChangingBuildStatusErrorHandler", result: "UNSTABLE"]],
      statusResultSource: [ $class: "ConditionalStatusResultSource", results: [[$class: "AnyBuildResult", message: message, state: state]] ]
  ]);
}

pipeline {
  agent {
    node {
      label 'master'
      customWorkspace "F:\\UE5\\UE5_Source\\Projects\\TargetVector" //use backward slashes to avoid problems with how Windows uses directories!!
    }
  }//^all this is necessary to run the build in a special workspace.
  environment {
    ue5Path = "F:\\UE5\\UE5_Source"
    ue5Project = "TargetVector"
    ue5ProjectDisplayName = "Target Vector"
    ueProjectFileName = "${ue5Project}.uproject"
    testSuiteToRun = "Game."//the '.' is used to run all tests inside the prettyname. The automation system searches for everything that has 'Game.' in it, so otherGame.'s tests would run too...
    testReportFolder = "Saved\\TestReport"
    testsLogName = "RunTests.log"
    pathToTestsLog = "${env.WORKSPACE}" + "\\Saved\\Logs\\" + "${testsLogName}"
    codeCoverageReportName="Saved\\TestReport\\CodeCoverageReport.xml"
    tvDiscordWebhook = "${TVDISCORDWEBHOOK}"
  }
  stages {
    stage('Building') {
      steps {
        discordSend description: ":test_tube:  ${ue5ProjectDisplayName} build ${env.BUILD_DISPLAY_NAME} Started", footer: "Commit ${GIT_COMMIT} started build ${env.BUILD_DISPLAY_NAME} on ${env.BRANCH_NAME} at node ${env.NODE_NAME}", link: env.BUILD_URL, result: "ABORTED", image: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector_full.png", thumbnail: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector.png", title: "${ue5ProjectDisplayName} build ${env.BUILD_DISPLAY_NAME} started", webhookURL: "${tvDiscordWebhook}"

        bat "BuildWithoutCooking.bat \"${ue5Path}\" \"${env.WORKSPACE}\" \"${ueProjectFilename}\""//builds our project
      }
      post {
        success {
          echo 'Build Stage Successful.'
        }
        failure {
          echo 'Build Stage Unsuccessful.'
        }
      }
    }

    stage('Testing') {
      steps {
        echo 'Testing Stage Started.'
        script {
          if(env.BRANCH_NAME == 'main') {
            echo 'Push to main branch recognized. Starting Tests and Code Coverage.'
            bat "TestRunnerAndCodeCoverage.bat \"${ue5Path}\" \"${env.WORKSPACE}\" \"${ueProjectFilename}\" \"${testSuiteToRun}\" \"${testReportFolder}\" \"${testsLogName}\" \"${codeCoverageReportName}\"" // runs the tests and performs code coverage
          }
          else {
            bat "TestRunner.bat \"${ue5Path}\" \"${env.WORKSPACE}\" \"${ueProjectFilename}\" \"${testSuiteToRun}\" \"${testReportFolder}\" \"${testsLogName}\"" // runs the tests
          }
        }
      }
      post {
        success {
          echo 'Testing Stage Successful.'
        }
        failure {
          echo 'Testing Stage Unsuccessful.'
        }
      }
    }

  }
  post {
    always{
      echo 'Tests finished. Printing log.'
      bat "type ${pathToTestsLog}"
      echo 'Formatting Test Report from JSON to JUnit XML'
      formatUnitTests()
 
      discordSend description: ":bar_chart:  Total Tests: ${testReportSummary.totalCount}, Failures: ${testReportSummary.failCount}, Skipped: ${testReportSummary.skipCount}, Passed: ${testReportSummary.passCount}", footer: "${GIT_COMMIT} on ${env.BRANCH_NAME}", result: currentBuild.currentResult, link: env.BUILD_URL, thumbnail: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector.png", title: "*Tests Report Summary* for  build ${env.BUILD_DISPLAY_NAME} on ${env.BRANCH_NAME}", webhookURL: "${tvDiscordWebhook}"
       
       script {
       if (env.BRANCH_NAME == 'main') {
           echo "Publish Code Coverage Report."
           cobertura coberturaReportFile: "${codeCoverageReportName}"
           }
       }

      echo 'Cleaning up workspace:'
      echo '-checking current workspace.'
      powershell label: 'show workspace', script: 'dir $WORKSPACE'
      bat 'git reset --hard'//resets to HEAD, to the commit in the cloned repository.
      bat 'git clean -dffx .'//removes untracked files.
      echo '-checking clean workspace.'
      powershell label: 'show workspace', script: 'dir $WORKSPACE'

      // echo 'Sending build status notification to Discord:'
    }
    success{
      setBuildStatus("Build succeeded", "SUCCESS");
    	discordSend description: ":checkered_flag:  ${ue5ProjectDisplayName} build ${env.BUILD_DISPLAY_NAME} Successful", footer: "${GIT_COMMIT} build ${env.BUILD_DISPLAY_NAME} succeeded on ${env.BRANCH_NAME} at node ${env.NODE_NAME}", link: env.BUILD_URL, result: currentBuild.currentResult, image: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector_full.png", thumbnail: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector.png", title: "${ue5ProjectDisplayName} (${env.BUILD_DISPLAY_NAME}) Build Successful!", webhookURL: "${tvDiscordWebhook}"
    }
    unstable{
      setBuildStatus("Build unstable", "UNSTABLE");
    	discordSend description: ":warning:  ${ue5ProjectDisplayName} build ${env.BUILD_DISPLAY_NAME} Unstable", footer: "Commit ${GIT_COMMIT} build ${env.BUILD_DISPLAY_NAME} unstable on ${env.BRANCH_NAME} at node ${env.NODE_NAME}", link: env.BUILD_URL, result: currentBuild.currentResult, image: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector_full.png", thumbnail: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector.png", title: "${ue5ProjectDisplayName} (${env.BUILD_DISPLAY_NAME}) Build Unstable", webhookURL: "${tvDiscordWebhook}"
    }
    failure{
      setBuildStatus("Build failed", "FAILURE");
    	discordSend description: ":boom:  ${ue5ProjectDisplayName} build ${env.BUILD_DISPLAY_NAME} Failed", footer: "Commit ${GIT_COMMIT} build ${env.BUILD_DISPLAY_NAME} failed on ${env.BRANCH_NAME} at node ${env.NODE_NAME}", link: env.BUILD_URL, result: currentBuild.currentResult, image: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector_full.png", thumbnail: "https://raw.githubusercontent.com/Voidware-Prohibited/ALS-Refactored-EOS/main/TargetVector.png", title: "${ue5ProjectDisplayName} (${env.BUILD_DISPLAY_NAME}) Build Failed", webhookURL: "${tvDiscordWebhook}"
    }
  }
}

import groovy.json.JsonSlurper
import groovy.xml.MarkupBuilder

def testReportSummary = 'to be populated...'

def formatUnitTests() {
        try {
          convertTestsReport()
          testReportSummary = junit "${testReportFolder}\\junit.xml"
        }
        catch (Exception e){
          echo "couldn't generate JUnit Test Report to ${testReportFolder}..."
        }
}

def convertTestsReport() {
    def jsonReport = readFile file: "${testReportFolder}\\index.json", encoding: "UTF-8"
    // Needed because the JSON is encoded in UTF-8 with BOM

    jsonReport = jsonReport.replace( "\uFEFF", "" );

    def xmlContent = transformReport( jsonReport )

    writeFile file: "${testReportFolder}\\junit.xml", text: xmlContent.toString()
}

@NonCPS//atomic method
def transformReport( String jsonContent ) {

    def parsedReport = new JsonSlurper().parseText( jsonContent )
    
    def jUnitReport = new StringWriter()
    def builder = new MarkupBuilder( jUnitReport )

    builder.doubleQuotes = true
    builder.mkp.xmlDeclaration version: "1.0", encoding: "utf-8"

    builder.testsuite( tests: parsedReport.succeeded + parsedReport.failed, failures: parsedReport.failed, time: parsedReport.totalDuration ) {
      for ( test in parsedReport.tests ) {
        builder.testcase( name: test.testDisplayName, classname: test.fullTestPath, status: test.state ) {
          if(test.state == "Fail") {
            for ( entry in test.entries ) { 
              if(entry.event.type == "Error") {
                builder.failure( message: entry.event.message, type: entry.event.type, entry.filename + " " + entry.lineNumber )
              }
            }
          }
        }
      }
    } 

    return jUnitReport.toString()
}