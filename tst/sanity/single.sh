#!/bin/bash


correct_result='"errno" : 0'; 

test_channel="test_$RANDOM";

INSTANCE="demo64.geo2tag.org:80"
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

#response_incorrect_json_test=`curl   -d '{"login":"Markpassword":"test"}'  http://${INSTANCE}/service/login`;
#echo "Incorrect Json test - $response_incorrect_json_test"
#correct_result_incorrect_json='{ "errno" : 9 }';
#if ! echo $response_incorrect_json_test | grep -q -s -F "$correct_result_incorrect_json"  ; 
#then
#	echo "Fail at incorrect json test"
#	exit 1
#fi

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
response_write_tag_test=`curl   -d "{ \"auth_token\" : "$auth_token", \"channel\" : \"$test_channel\", \"description\" : \"\", \"altitude\" : $test_altitude , \"latitude\" : 0.0,\"link\" : \"\", \"longitude\" : 0.0, \"time\" : \"$test_time\", \"title\" : \"\" }"  http://${INSTANCE}/service/writeTag`;
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

response_unsubscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/unsubscribe`;
echo "Unsubscribe test - $response_unsubscribe"
if ! echo $response_unsubscribe | grep -q -s -F "$correct_result"  ;
then
        echo "Fail at unsubscribe test"
        exit 1
fi

response_quit_session_test=`curl   -d "{\"auth_token\":"$auth_token"}"  http://${INSTANCE}/service/quitSession`;
echo "Quit session test - $response_quit_session_test"
correct_result_quit_session='{ "errno" : 0 }';
if ! echo $response_quit_session_test | grep -q -s -F "$correct_result_quit_session"  ; 
then
	echo "Fail at quitSession test"
	exit 1
fi

#session_error_with_add_channel=`curl   -d "{\"auth_token\":"$auth_token", \"name\":\"$test_channel\", \"description\":\"\", \"url\":\"\", \"activeRadius\":30}"  http://${INSTANCE}/service/addChannel`;
#echo "Session error with add channel - $session_error_with_add_channel"
#correct_result_session_error_with_add_channel='{ "errno" : 1 }';
#echo "$response_add_channel"
#if ! echo $session_error_with_add_channel | grep -q -s -F "$correct_result_session_error_with_add_channel"  ;
#then
#	echo "Fail at sessionError with addChannel test"
#	exit 1
#fi

#session_error_with_subscribe=`curl   -d "{\"auth_token\":"$auth_token", \"channel\":\"$test_channel\"}" http://${INSTANCE}/service/subscribe`;
#echo "Session error with subscribe - $session_error_with_subscribe"
#correct_result_session_error_with_subscribe='{ "errno" : 1 }';
#if ! echo $session_error_with_subscribe | grep -q -s -F "$correct_result_session_error_with_subscribe"  ;
#then
#        echo "Fail at sessionError with subscribe test"
#        exit 1
#fi

#### Commented because addUser and deleteUser are stubs now
#	response_add_user_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" http://${INSTANCE}/service/addUser`;
#	echo "Add user test - $response_add_user_test"
#	if ! echo $response_add_user_test | grep -q -s -F "$correct_result"  ;
#	then
#		echo "Fail at addUser test"
#		exit 1
#	fi
#
#	echo "$test_channel"
#	response_delete_user_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" http://${INSTANCE}/service/deleteUser`;
#	echo "Delete user test - $response_delete_user_test"
#	#response_check_delete_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\"}" http://${INSTANCE}/service/login`;
#	echo "Check delete test - $response_check_delete_test"
#	if ! echo $response_delete_user_test | grep -q -s -F "$correct_result"  ;
#	then
#		echo "Fail at deleteUser test"
#		exit 1
#	fi
#	if  echo $response_check_delete_test | grep -q -s -F "$correct_result"  ;
#	then
#		echo "Fail at checkDelete test"
#					echo $response_check_delete_test
#		#exit 1
#	fi

#random_number=$((RANDOM%1000000));
#rand_user=user_$random_number;
#rand_email=email_$random_number;
#response_add_rand_user_test=`curl   -d "{\"login\":\"$rand_user\",\"password\":\"test\",\"email\":\"$rand_email\"}" http://${INSTANCE}/service/addUser`;
#echo "Add random user test - $response_add_rand_user_test"
#if ! echo $response_add_rand_user_test | grep -q -s -F "$correct_result"  ;
#then
#        echo "Fail at add random user test"
#        exit 1
#fi


#response_restore_password_test=`curl   -d "{\"email\":\"$rand_email\"}"  http://${INSTANCE}/service/restorePassword`;
#echo $response_restore_password_test;
#echo $ "Restore Password test - $response_restore_password_test"
#correct_result_restore_password="\"errno\" : 0";
#if ! echo $response_restore_password_test | grep -q -s -F "$correct_result_restore_password"  ; 
#then
#        echo "Fail at restorePassword test"
#        exit 1
#fi


response_build_test=`curl   -d "{\"login\":\"$test_channel\",\"password\":\"test\",\"email\":\"11@11.ru\"}" http://${INSTANCE}/service/build`;
echo "build test - $response_build_test"
if ! echo $response_add_user_test | grep -q -s -F "$correct_result"  ;
then
        echo "Build test"
        exit 1
fi


echo "Success"

