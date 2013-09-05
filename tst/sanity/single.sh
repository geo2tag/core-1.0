#!/bin/bash

if [ $# -ne "1" ]
then
	echo "Usage: ./single.sh server:port"
	exit 1
fi

correct_result='"errno" : 0'; 
errno_success="0";

integration_db="integration_testing_db";
test_channel="test_$RANDOM";

INSTANCE=$1

function checkResponseErrno(){

	#Usage: ./checkResponseErrno response errno error_message

	response=$1;
	errno=$2;
	error_message=$3;
	correct_result_with_errno="\"errno\" : $errno";


	if ! echo $response | grep -q -s -F "$correct_result_with_errno" ; 
	then
		echo $error_message
		exit 1	
	fi
}

function setTestingDb(){

	#Usage: setTestingDb auth_token

	echo "Do setDb for $integration_db"
	token=$1;
	set_testing_db_result=`curl -d "{\"auth_token\":"$token", \"db_name\":\"$integration_db\"}" http://${INSTANCE}/service/setDb`;
	echo "{\"auth_token\":"$token", \"db_name\":\"$integration_db\"}"
	echo "$set_testing_db_result"

	checkResponseErrno "$set_testing_db_result" $errno_success "Error during setTestingDb($token)"

}


response_login_test=`curl   -d '{"login":"Paul","password":"test"}'  http://${INSTANCE}/service/login`;
echo $ "Login test - $response_login_test"
auth_token=`echo $response_login_test | grep -Po '"\w+"' | grep -v "auth_token" | grep -v "errno"`;
checkResponseErrno "$response_login_test" $errno_success "Fail at login test"

setTestingDb $auth_token

response_CI_test=`curl   -d '{"login":"mark","password":"test"}'  http://${INSTANCE}/service/login`;
echo "CI test - $response_CI_test"
checkResponseErrno "$response_CI_test" $errno_success "Fail at caseinsentivenes test"

response_add_channel=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  http://${INSTANCE}/service/addChannel`;
echo "Add channel test - $response_add_channel"
checkResponseErrno "$response_add_channel" $errno_success "Fail at addChannel test"

response_subscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/subscribe`;
echo "Subscribe test - $response_subscribe"
checkResponseErrno "$response_subscribe" $errno_success  "Fail at subscribe test"

response_subscribed=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/subscribed`;
echo "Subscribe test - $response_subscribed"
checkResponseErrno "$response_subscribed" $errno_success "Fail at subscribed test"

response_owned=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/owned`;
echo " test [owned] - $response_owned"
checkResponseErrno "$response_owned" $errno_success "Fail at owned test"

response_errno_info_test=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/errnoInfo`;
echo " test [ErrnoInfo] - $response_errno_info_test"
checkResponseErrno "$response_errno_info_test" $errno_success "Fail at errnoInfo test"

response_incorrect_json_test=`curl   -d '{"login":"Markpassword":"test"}'  http://${INSTANCE}/service/login`;
echo "Incorrect Json test - $response_incorrect_json_test"
correct_result_incorrect_json='"errno" : 9';
errno_incorrect_json="9";
checkResponseErrno "$response_incorrect_json_test" $errno_incorrect_json "Fail at incorrect json test"

response_incorrect_url_test=`curl   -d ''  http://${INSTANCE}/service/incorrect_url`;
echo "Incorrect url test - $response_incorrect_url_test"
correct_result_incorrect_url='{ "errno" : 8 }';
errno_incorrect_url="8";
checkResponseErrno "$response_incorrect_url_test" $errno_incorrect_url "Fail at incorrect url test"

test_altitude=$((RANDOM%100)).0;
test_time=`date +'%d %m %Y %H:%M:%S.300'`;
echo "test time = $test_time"
echo "Test alt:$test_altitude"
response_write_tag_test=`curl -d "{ \"auth_token\" : "$auth_token", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  http://${INSTANCE}/service/writeTag`;
echo "Write tag test - $response_write_tag_test"
checkResponseErrno "$response_write_tag_test" $errno_success "Fail at writeTag test"

default_db_name="default"
response_setdb_test_1=`curl -d "{ \"auth_token\" : "$auth_token", \"db_name\" : \"$default_db_name\" }"  http://${INSTANCE}/service/setDb`;
echo "SetDb test (attempt to change db to default value) - $response_setdb_test_1"
checkResponseErrno "$response_setdb_test_1" $errno_success "Fail at setDb_1 test"

setTestingDb $auth_token

incorrect_db_name="incorrect_database_name";
db_does_not_exist_result="\"errno\" : 22";
errno_db_does_not_exist="22";
response_setdb_test_2=`curl -d "{ \"auth_token\" : "$auth_token", \"db_name\" : \"$incorrect_db_name\" }"  http://${INSTANCE}/service/setDb`;
echo "SetDb test (attempt to change db_name to not existing value) - $response_setdb_test_2"
checkResponseErrno "$response_setdb_test_2" $errno_db_does_not_exist "Fail at setDb_2 test"

response_altitude_test=`curl   -d "{\"auth_token\":"$auth_token",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  http://${INSTANCE}/service/loadTags`;
echo "Alt test - $response_altitude_test "
if ! echo $response_altitude_test | grep -q -s -F "$test_altitude"  ;
then
        echo "Fail at altitude test"
        exit 1
fi

response_build_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" http://${INSTANCE}/service/build`;
echo "build test - $response_build_test"
checkResponseErrno "$response_build_test"  $errno_success "Fail at build test"

response_unsubscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/unsubscribe`;
echo "Unsubscribe test - $response_unsubscribe"
checkResponseErrno "$response_unsubscribe"   $errno_success "Fail at unsubscribe test"

response_GT_935_test_1=`curl -d "{\"login\":\"Mark\') OR id=1;--\",\"password\":\"tkkkkest\"}" http://${INSTANCE}/service/login`;
correct_result_GT_935='10';
errno_incorrect_credentials="10";
checkResponseErrno  "$response_GT_935_test_1" $errno_incorrect_credentials "Fail at GT_935 test #1"

response_GT_935_test_2=`curl -d "{\"login\":\"Mark\' OR id=1;--\",\"password\":\"tkkkkest\"}" http://${INSTANCE}/service/login`;
echo "$response_GT_935_test_2"
checkResponseErrno "$response_GT_935_test_2" $errno_incorrect_credentials "Fail at GT_935 test #2"


response_GT_960_test_1=`curl -d "{\"auth_token\":"$auth_token", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"  http://${INSTANCE}/service/filterCircle`;
echo "{\"auth_token\":\"$auth_token\", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"
echo  "response_GT_960_test_1 - $response_GT_960_test_1"
response_GT_960_test_correct_result='"errno" : 9';
checkResponseErrno "$response_GT_960_test_1" $errno_incorrect_json "Fail at GT-960 test #1"


response_GT_960_test_2=`curl -d "{\"auth_token\":"$auth_token",  \"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0} }" http://${INSTANCE}/service/filterRectangle`;
echo  "response_GT_960_test_2 - $response_GT_960_test_2"
checkResponseErrno "$response_GT_960_test_2" $errno_incorrect_json "Fail at GT-960 test #2"

response_GT_960_test_3=`curl -d "{\"auth_token\":"$auth_token",  \"polygon\" : [{ \"number\":0,\"latitude\":0.0, \"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ] }" http://${INSTANCE}/service/filterPolygon`;
echo  "response_GT_960_test_3 - $response_GT_960_test_3"
checkResponseErrno "$response_GT_960_test_3" $errno_incorrect_json "Fail at GT-960 test #3"

response_GT_960_test_4=`curl -d "{\"auth_token\":"$auth_token", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterCylinder`;
echo  "response_GT_960_test_4 - $response_GT_960_test_4"
checkResponseErrno "$response_GT_960_test_4" $errno_incorrect_json "Fail at GT-960 test #4"

response_GT_960_test_5=`curl -d "{ \"auth_token\":"$auth_token",\"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0}, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterBox`;
echo  "response_GT_960_test_5 - $response_GT_960_test_5"
checkResponseErrno "$response_GT_960_test_5" $errno_incorrect_json "Fail at GT-960 test #5"

response_GT_960_test_6=`curl -d "{ \"auth_token\":"$auth_token", \"polygon\" : [{ \"number\":0,\"latitude\":0.0,\"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ], \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterFence`;
echo  "response_GT_960_test_6 - $response_GT_960_test_6"
checkResponseErrno "$response_GT_960_test_6" $errno_incorrect_json "Fail at GT-960 test #6"

#### Filter tests

response_filter_circle_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"  http://${INSTANCE}/service/filterCircle`;
checkResponseErrno "$response_filter_circle_test" $errno_success "Fail at filterCircle test"

response_filter_rectangle_test=`curl -d "{\"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,  \"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0} }" http://${INSTANCE}/service/filterRectangle`;
checkResponseErrno "$response_filter_rectangle_test" $errno_success "Fail at filterRectangle test"

response_filter_polygon_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" , \"polygon\" : [{ \"number\":0,\"latitude\":0.0, \"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ] }" http://${INSTANCE}/service/filterPolygon`;
checkResponseErrno "$response_filter_polygon_test" $errno_success "Fail at filterPolygon test"

response_filter_cylinder_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterCylinder`;
checkResponseErrno "$response_filter_cylinder_test" $errno_success "Fail at filterCylinder test"

response_filter_box_test=`curl -d "{ \"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0}, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterBox`;
checkResponseErrno "$response_filter_box_test" $errno_success "Fail at filterBox test"

response_filter_fence_test=`curl -d "{ \"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"polygon\" : [{ \"number\":0,\"latitude\":0.0,\"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ], \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterFence`;
checkResponseErrno "$response_filter_fence_test" $errno_success "Fail at filterFence test"

# Check that alterChannel works normaly
response_alter_channel_1=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"field\":\"url\", \"value\":\"test_url\"}" http://${INSTANCE}/service/alterChannel`;
echo "AlterChannel test_2 - $response_alter_channeli_1"
checkResponseErrno "$response_alter_channel_1"   $errno_success "Fail at alterChannel_1 test"

# Check that field name is checked
response_alter_channel_2=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"field\":\"wrong field name\", \"value\":\"test_url\"}" http://${INSTANCE}/service/alterChannel`;
echo "AlterChannel test_2 - $response_alter_channel_2"
checkResponseErrno "$response_alter_channel_2"   $errno_incorrect_json "Fail at alterChannel_2 test"

# Check that ownership is checked
response_alter_channel_3=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"Rom\", \"field\":\"url\", \"value\":\"test_url\"}" http://${INSTANCE}/service/alterChannel`;
echo "AlterChannel test_3 - $response_alter_channel_3"
errno_channel_not_owned="23";
checkResponseErrno "$response_alter_channel_3"   $errno_channel_not_owned "Fail at alterChannel_3 test"

# Check that channel existance is checked
response_alter_channel_4=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"not_existing_channel\", \"field\":\"url\", \"value\":\"test_url\"}" http://${INSTANCE}/service/alterChannel`;
echo "AlterChannel test_4 - $response_alter_channel_4"
errno_channel_does_not_exist="5";
checkResponseErrno "$response_alter_channel_4"   $errno_channel_does_not_exist "Fail at alterChannel_4 test"

response_quit_session_test=`curl   -d "{\"auth_token\":"$auth_token"}"  http://${INSTANCE}/service/quitSession`;
echo "Quit session test auth_token=$auth_token, - $response_quit_session_test"
checkResponseErrno "$response_quit_session_test" $errno_success "Fail at quitSession test"

echo "${INSTANCE}"

echo "Success"

