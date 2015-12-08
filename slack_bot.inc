<?php

use CL\Slack\Transport\ApiClient;
use CL\Slack\Payload\ChatPostMessagePayload;
use CL\Slack\Payload\GroupsCreatePayload;
use CL\Slack\Payload\GroupsInvitePayload;
use CL\Slack\Payload\UsersListPayload;
use CL\Slack\Payload\GroupsListPayload;
use CL\Slack\Payload\GroupsArchivePayload;

function slack_bot_get_client() {
    if ($token = variable_get('slack_bot_token', FALSE)) {
        return new ApiClient($token);
    }
}

function slack_bot_get_user_nick($user) {
    return variable_get('slack_bot_nickname_' . $user->uid, FALSE);
}

function slack_bot_get_user_dob($user) {
    if ($dob = variable_get('slack_bot_dob_' . $user->uid, FALSE)) {
        return strtotime(str_replace('/', '-', $dob));   
    }
}

function slack_bot_get_user_group_name($user) {
    return '_' . strtolower(slack_bot_get_user_nick($user)) . '_db_' . date('d_m', slack_bot_get_user_dob($user)) . '_' . date('Y');
}

// Create a new 'Happy Birthday' private channel
function slack_bot_create_group($client, $account) {
    $groupName = slack_bot_get_user_group_name($account);
    $nick = slack_bot_get_user_nick($account);

    $payload = new GroupsCreatePayload();
    $payload->setName($groupName);
    $response = $client->send($payload);
    if ($response->isOk()) {
        $group = $response->getGroup();

        $payload   = new UsersListPayload();
        $response  = $client->send($payload);
        if ($response->isOk()) {
            foreach ($response->getUsers() as $user) {
                if ($nick != $user->getName()) {
                    $payload = new GroupsInvitePayload();
                    $payload->setGroupId($group->getId());
                    $payload->setUserId($user->getId());
                    $response = $client->send($payload);
                }
            }
    
            $payload = new ChatPostMessagePayload();
            $payload->setChannel($groupName);
            $payload->setMessage(variable_get('slack_bot_message_' . $account->uid, variable_get('slack_bot_message', t('Happy Birthday!'))));
            $client->send($payload);
        }
    }
    else {
        watchdog('Slack', $response->getErrorExplanation(). ' !name', array('!name' => $groupName), WATCHDOG_ERROR);
    }
}

// Remove an old 'Happy Birthday' private channel
function slack_bot_remove_group($client, $account) {
    $groupName = slack_bot_get_user_group_name($account);
    $payload = new GroupsListPayload();
    $payload->setExcludeArchived(TRUE);
    $response  = $client->send($payload);
    
    if ($response->isOk()) {
        foreach ($response->getGroups() as $channel) {
            dsm($channel->getName());
            dsm($groupName);
            if ($channel->getName() == $groupName) {
                $payload = new GroupsArchivePayload();
                $payload->setGroupId($channel->getId());
                
                $response = $client->send($payload);
                
                if ($response->isOk()) {
                    watchdog('Slack', t('!group has been archived.', array('!group' => $groupName)));
                } else {
                    watchdog('Slack', $response->getErrorExplanation(), array(), WATCHDOG_ERROR);
                }
                break;
            }
        }
    }
}
