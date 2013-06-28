#!/bin/bash


correct_result='"errno" : 0'; 

test_channel="test_$RANDOM";

INSTANCE="demo64.geo2tag.org:80"

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

response_incorrect_json_test=`curl   -d '{"login":"Markpassword":"test"}'  http://${INSTANCE}/service/login`;
echo "Incorrect Json test - $response_incorrect_json_test"
correct_result_incorrect_json='9';
if ! echo $response_incorrect_json_test | grep -q -s -F "$correct_result_incorrect_json"  ; 
then
	echo "Fail at incorrect json test"
	echo "expected "${correct_result_incorrect_json}
	echo "received "${response_incorrect_json_test}
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

response_subscribe_test=`curl   -d "{\"auth_token\":"$auth_token",\"channel\":\"$test_channel\"}"  http://${INSTANCE}/service/subscribe`;
echo "Subscribe test - $response_subscribe_test"
if ! echo $response_subscribe_test | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at Subscribe test"
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


response_altitude_test=`curl   -d "{\"auth_token\":"$auth_token",\"latitude\":0.0, \"longitude\":0.0, \"radius\":10.0}"  http://${INSTANCE}/service/loadTags`;
echo "Alt test - $response_altitude_test "
if ! echo $response_altitude_test | grep -q -s -F "$test_altitude"  ;
then
        echo "Fail at altitude test"
        exit 1
fi

response_GT_935_test=`curl -d "{\"login\":\"Mark\') OR id=1;--\",\"password\":\"tkkkkest\"}" http://localhost/service/login`;
correct_result_GT_935='10';
if ! echo $response_GT_935_test | grep -q -s -F "$correct_result_GT_935"  ;
then
        echo "Fail at GT_935 test"
        exit 1
fi


echo "Success"

