#!/bin/bash

if [ $# -ne "1" ]
then
	echo "Usage: ./single.sh server:port"
	exit 1
fi

correct_result='"errno" : 0'; 

test_channel="test_$RANDOM";

INSTANCE=$1
#"localhost"
#INSTANCE="tracks.osll.spb.ru:81"

response_login_test=`curl   -d '{"login":"Paul","password":"test"}'  http://${INSTANCE}/service/login`;
echo $ "Login test - $response_login_test"
auth_token=`echo $response_login_test | grep -Po '"\w+"' | grep -v "auth_token" | grep -v "errno"`;
correct_result_login="\"errno\" : 0";
#correct_result_login="{ \"auth_token\" : \"mmmmmmmmmm\", \"errno\" : 0 }";
if ! echo $response_login_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at login test"
	exit 1
fi

response_CI_test=`curl   -d '{"login":"mark","password":"test"}'  http://${INSTANCE}/service/login`;
echo "CI test - $response_CI_test"
if ! echo $response_CI_test | grep -q -s -F "$correct_result_login"  ; 
then
	echo "Fail at caseinsentivenes test"
fi

response_add_channel=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  http://${INSTANCE}/service/addChannel`;
echo "Add channel test - $response_add_channel"
if ! echo $response_add_channel | grep -q -s -F "$correct_result"  ;
then
	echo "Fail at addChannel test"
	exit 1
fi

response_subscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/subscribe`;
echo "Subscribe test - $response_subscribe"
if ! echo $response_subscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at subscribe test"
        exit 1
fi

response_subscribed=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/subscribed`;
echo "Subscribe test - $response_subscribed"
if ! echo $response_subscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at subscribed test"
        exit 1
fi

response_owned=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/owned`;
echo " test [owned] - $response_owned"
if ! echo $response_owned | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at owned test"
#        exit 1
fi

response_errno=`curl   -d "{\"auth_token\":"$auth_token"}" http://${INSTANCE}/service/errnoInfo`;
echo " test [ErrnoInfo] - $response_errno"

response_incorrect_json_test=`curl   -d '{"login":"Markpassword":"test"}'  http://${INSTANCE}/service/login`;
echo "Incorrect Json test - $response_incorrect_json_test"
correct_result_incorrect_json='"errno" : 9';
if ! echo $response_incorrect_json_test | grep -q -s -F "$correct_result_incorrect_json"  ; 
then
	echo "Fail at incorrect json test"
	exit 1
fi

response_incorrect_url_test=`curl   -d ''  http://${INSTANCE}/service/incorrect_url`;
echo "Incorrect url test - $response_incorrect_url_test"
correct_result_incorrect_url='{ "errno" : 8 }';
if ! echo $response_incorrect_url_test | grep -q -s -F "$correct_result_incorrect_url"  ; 
then
	echo "Fail at incorrect url test"
	exit 1
fi

test_altitude=$((RANDOM%100)).0;
test_time=`date +'%d %m %Y %H:%M:%S.300'`;
echo "test time = $test_time"
echo "Test alt:$test_altitude"
response_write_tag_test=`curl -d "{ \"auth_token\" : "$auth_token", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  http://${INSTANCE}/service/writeTag`;
echo "Write tag test - $response_write_tag_test"
if ! echo $response_write_tag_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at writeTag test"
        exit 1
fi

default_db_name="default"
response_setdb_test_1=`curl -d "{ \"auth_token\" : "$auth_token", \"db_name\" : \"$default_db_name\" }"  http://${INSTANCE}/service/setDb`;
echo "SetDb test (attempt to change db to default value) - $response_setdb_test_1"
if ! echo $response_setdb_test_1 | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at setDb_1 test"
        exit 1
fi

incorrect_db_name="incorrect_database_name"
db_does_not_exist_result="22"
response_setdb_test_2=`curl -d "{ \"auth_token\" : "$auth_token", \"db_name\" : \"$incorrect_db_name\" }"  http://${INSTANCE}/service/setDb`;
echo "SetDb test (attempt to change db to default value) - $response_setdb_test_2"
if ! echo $response_setdb_test_2 | grep -q -s -F "$db_does_not_exist_result"  ;
then
        echo "Fail at setDb_2 test"
        exit 1
fi

response_altitude_test=`curl   -d "{\"auth_token\":"$auth_token",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  http://${INSTANCE}/service/loadTags`;
echo "Alt test - $response_altitude_test "
if ! echo $response_altitude_test | grep -q -s -F "$test_altitude"  ;
then
        echo "Fail at altitude test"
        exit 1
fi

response_build_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" http://${INSTANCE}/service/build`;
echo "build test - $response_build_test"
if ! echo $response_build_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at build test"
        exit 1
fi

test_altitude=$((RANDOM%100)).0;
test_time=`date +'%d %m %Y %H:%M:%S.300'`;
echo "test time = $test_time"
echo "Test alt:$test_altitude"
response_write_tag_test=`curl   -d "{ \"auth_token\" : "$auth_token", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  http://${INSTANCE}/service/writeTag`;
echo "Write tag test - $response_write_tag_test"
if ! echo $response_write_tag_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at writeTag test"
        exit 1
fi

response_unsubscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/unsubscribe`;
echo "Unsubscribe test - $response_unsubscribe"
if ! echo $response_unsubscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at unsubscribe test"
        exit 1
fi

response_GT_935_test_1=`curl -d "{\"login\":\"Mark\') OR id=1;--\",\"password\":\"tkkkkest\"}" http://${INSTANCE}/service/login`;
correct_result_GT_935='10';
if ! echo $response_GT_935_test_1 | grep -q -s -F "$correct_result_GT_935"  ;
then
        echo "Fail at GT_935 test #1"
        exit 1
fi

response_GT_935_test_2=`curl -d "{\"login\":\"Mark\' OR id=1;--\",\"password\":\"tkkkkest\"}" http://${INSTANCE}/service/login`;
echo "$response_GT_935_test_2"
if ! echo $response_GT_935_test_2 | grep -q -s -F "$correct_result_GT_935"  ;
then
        echo "Fail at GT_935 test #2"
        exit 1
fi


response_GT_960_test_1=`curl -d "{\"auth_token\":"$auth_token", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"  http://${INSTANCE}/service/filterCircle`;
echo "{\"auth_token\":\"$auth_token\", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"
echo  "response_GT_960_test_1 - $response_GT_960_test_1"
response_GT_960_test_correct_result='"errno" : 9';
if ! echo $response_GT_960_test_1 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #1"
        exit 1
fi


response_GT_960_test_2=`curl -d "{\"auth_token\":"$auth_token",  \"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0} }" http://${INSTANCE}/service/filterRectangle`;
echo  "response_GT_960_test_2 - $response_GT_960_test_2"
if ! echo $response_GT_960_test_2 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #2"
        exit 1
fi

response_GT_960_test_3=`curl -d "{\"auth_token\":"$auth_token",  \"polygon\" : [{ \"number\":0,\"latitude\":0.0, \"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ] }" http://${INSTANCE}/service/filterPolygon`;
echo  "response_GT_960_test_3 - $response_GT_960_test_3"
if ! echo $response_GT_960_test_3 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #3"
        exit 1
fi

response_GT_960_test_4=`curl -d "{\"auth_token\":"$auth_token", \"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterCylinder`;
echo  "response_GT_960_test_4 - $response_GT_960_test_4"
if ! echo $response_GT_960_test_4 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #4"
        exit 1
fi

response_GT_960_test_5=`curl -d "{ \"auth_token\":"$auth_token",\"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0}, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterBox`;
echo  "response_GT_960_test_5 - $response_GT_960_test_5"
if ! echo $response_GT_960_test_5 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #5"
        exit 1
fi

response_GT_960_test_6=`curl -d "{ \"auth_token\":"$auth_token", \"polygon\" : [{ \"number\":0,\"latitude\":0.0,\"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ], \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterFence`;
echo  "response_GT_960_test_6 - $response_GT_960_test_6"
if ! echo $response_GT_960_test_6 | grep -q -s -F "$response_GT_960_test_correct_result"  ;
then
        echo "Fail at GT-960 test #6"
        exit 1
fi

#### Filter tests

response_filter_circle_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0 }"  http://${INSTANCE}/service/filterCircle`;
if ! echo $response_filter_circle_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterCircle test"
        exit 1
fi


response_filter_rectangle_test=`curl -d "{\"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,  \"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0} }" http://${INSTANCE}/service/filterRectangle`;
if ! echo $response_filter_rectangle_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterRectangle test"
        exit 1
fi

response_filter_polygon_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" , \"polygon\" : [{ \"number\":0,\"latitude\":0.0, \"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ] }" http://${INSTANCE}/service/filterPolygon`;
if ! echo $response_filter_polygon_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterPolygon test"
        exit 1
fi

response_filter_cylinder_test=`curl -d "{\"auth_token\":"$auth_token", \"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude\":60.1632, \"longitude\":24.8593,  \"radius\":30.0, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterCylinder`;
if ! echo $response_filter_cylinder_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterCylinder test"
        exit 1
fi

response_filter_box_test=`curl -d "{ \"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"latitude_shift\":{ \"latitude1\":0.0 ,\"latitude2\": 100.0}, \"longitude_shift\":{ \"longitude1\":0.0 ,\"longitude2\": 100.0}, \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterBox`;
if ! echo $response_filter_box_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterBox test"
        exit 1
fi
response_filter_fence_test=`curl -d "{ \"auth_token\":"$auth_token",\"time_from\":\"04 03 2011 15:33:47.630\", \"time_to\":\"31 12 2011 15:33:47.630\" ,\"polygon\" : [{ \"number\":0,\"latitude\":0.0,\"longitude\":0.0 }, { \"number\":1,\"latitude\":70.0, \"longitude\":0.0 }, { \"number\":2, \"latitude\":70.0, \"longitude\":100.0 } ], \"altitude_shift\":{ \"altitude1\":-1.0 ,\"altitude2\": 1.0} }" http://${INSTANCE}/service/filterFence`;
if ! echo $response_filter_fence_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at filterFence test"
        exit 1
fi


response_quit_session_test=`curl   -d "{\"auth_token\":"$auth_token"}"  http://${INSTANCE}/service/quitSession`;
echo "Quit session test auth_token=$auth_token, - $response_quit_session_test"
correct_result_quit_session='{ "errno" : 0 }';
if ! echo $response_quit_session_test | grep -q -s -F "$correct_result_quit_session"  ; 
then
	echo "Fail at quitSession test"
	exit 1
fi

echo "${INSTANCE}"

echo "Success"

