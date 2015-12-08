<?php

/**
 * Building form. Global settings.
 */
function slack_bot_configure_form($form, &$form_state) {
    $form['slack_bot_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Slack token'),
        '#description' => t('Enter your secret token'),
        '#default_value' => variable_get('slack_bot_token'),
        '#required' => TRUE,
    );
    $form['slack_bot_message'] = array(
        '#type' => 'textfield',
        '#title' => t('Message'),
        '#description' => t('Enter a message'),
        '#default_value' => variable_get('slack_bot_message'),
        '#required' => TRUE,
    );

    return system_settings_form($form);
}

/**
 * Building form. User settings.
 */
function slack_bot_user_configure_form($form, &$form_state, $user) {
    $form['slack_bot_nickname_' . $user->uid] = array(
        '#type' => 'textfield',
        '#title' => t('Nickname'),
        '#description' => t('Enter your nickname'),
        '#default_value' => variable_get('slack_bot_nickname_' . $user->uid),
        '#required' => TRUE,
    );
    $form['slack_bot_dob_' . $user->uid] = array(
        '#type' => 'textfield',
        '#title' => t('DOB'),
        '#description' => t('Enter your DOB'),
        '#default_value' => variable_get('slack_bot_dob_' . $user->uid),
        '#required' => TRUE,
    );
    $form['slack_bot_message_' . $user->uid] = array(
        '#type' => 'textfield',
        '#title' => t('Message'),
        '#description' => t('Enter a message'),
        '#default_value' => variable_get('slack_bot_message_' . $user->uid),
    );

    return system_settings_form($form);
}
