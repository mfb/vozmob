<?php
// $Id: og_views.views_default.inc,v 1.3 2008/10/29 19:59:45 dww Exp $

function vozmob_support_views_default_views() {

$view = new view;
$view->name = 'feature';
$view->description = '';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 3.0;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Historia Destacada';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = 1;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['hide_empty'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Image (field_image) */
$handler->display->display_options['fields']['field_image_fid']['id'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['table'] = 'node_data_field_image';
$handler->display->display_options['fields']['field_image_fid']['field'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_image_fid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_image_fid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_image_fid']['link_to_node'] = 0;
$handler->display->display_options['fields']['field_image_fid']['label_type'] = 'none';
$handler->display->display_options['fields']['field_image_fid']['format'] = 'feature_linked';
$handler->display->display_options['fields']['field_image_fid']['multiple']['group'] = 1;
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_number'] = '1';
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_reversed'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'by';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = '';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
/* Field: Node: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'node_revisions';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['body']['alter']['trim'] = 1;
$handler->display->display_options['fields']['body']['alter']['max_length'] = '280';
$handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['body']['alter']['html'] = 0;
$handler->display->display_options['fields']['body']['hide_empty'] = 0;
$handler->display->display_options['fields']['body']['empty_zero'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter: Node: Promoted to front page */
$handler->display->display_options['filters']['promote']['id'] = 'promote';
$handler->display->display_options['filters']['promote']['table'] = 'node';
$handler->display->display_options['filters']['promote']['field'] = 'promote';
$handler->display->display_options['filters']['promote']['value'] = '1';
$handler->display->display_options['filters']['promote']['expose']['operator'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'blog' => 'blog',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'feature';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Home';
$handler->display->display_options['menu']['description'] = 'Feature story';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'primary-links';

  $views[$view->name] = $view;

$view = new view;
$view->name = 'features';
$view->description = 'Recently featured stories';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 3.0;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Historias Recientemente Destacadas';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'mini';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Image (field_image) */
$handler->display->display_options['fields']['field_image_fid']['id'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['table'] = 'node_data_field_image';
$handler->display->display_options['fields']['field_image_fid']['field'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_image_fid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_image_fid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_image_fid']['link_to_node'] = 0;
$handler->display->display_options['fields']['field_image_fid']['label_type'] = 'none';
$handler->display->display_options['fields']['field_image_fid']['format'] = 'features_linked';
$handler->display->display_options['fields']['field_image_fid']['multiple']['group'] = 1;
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_number'] = '1';
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_reversed'] = 0;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'on';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['date_format'] = 'custom';
$handler->display->display_options['fields']['created']['custom_date_format'] = 'n/j/Y';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'by';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Upload: Attached files */
$handler->display->display_options['fields']['upload_fid']['id'] = 'upload_fid';
$handler->display->display_options['fields']['upload_fid']['table'] = 'node';
$handler->display->display_options['fields']['upload_fid']['field'] = 'upload_fid';
$handler->display->display_options['fields']['upload_fid']['exclude'] = TRUE;
$handler->display->display_options['fields']['upload_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['upload_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['upload_fid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['upload_fid']['hide_empty'] = 0;
$handler->display->display_options['fields']['upload_fid']['empty_zero'] = 0;
$handler->display->display_options['fields']['upload_fid']['link_to_file'] = 0;
$handler->display->display_options['fields']['upload_fid']['only_listed'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter: Node: Promoted to front page */
$handler->display->display_options['filters']['promote']['id'] = 'promote';
$handler->display->display_options['filters']['promote']['table'] = 'node';
$handler->display->display_options['filters']['promote']['field'] = 'promote';
$handler->display->display_options['filters']['promote']['value'] = '1';
$handler->display->display_options['filters']['promote']['expose']['operator'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'blog' => 'blog',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'features';
$handler->display->display_options['menu']['title'] = 'Home';
$handler->display->display_options['menu']['description'] = 'Feature story';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'primary-links';

/* Display: Feed */
$handler = $view->new_display('feed', 'Feed', 'feed_1');
$handler->display->display_options['style_plugin'] = 'rss';
$handler->display->display_options['style_options']['mission_description'] = FALSE;
$handler->display->display_options['row_plugin'] = 'node_rss';
$handler->display->display_options['path'] = 'features/feed';
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'block_1' => 'block_1',
  'default' => 0,
);

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['block_description'] = 'Recently featured stories';
$handler->display->display_options['block_caching'] = '8';

  $views[$view->name] = $view;

$view = new view;
$view->name = 'overlay';
$view->description = 'Overlay';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 3.0;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Image (field_image) */
$handler->display->display_options['fields']['field_image_fid']['id'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['table'] = 'node_data_field_image';
$handler->display->display_options['fields']['field_image_fid']['field'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_image_fid']['link_to_node'] = 0;
$handler->display->display_options['fields']['field_image_fid']['label_type'] = 'none';
$handler->display->display_options['fields']['field_image_fid']['format'] = 'overlay_default';
$handler->display->display_options['fields']['field_image_fid']['multiple']['group'] = 1;
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_reversed'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'by';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'on';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
/* Field: Node: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'node_revisions';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['body']['alter']['trim'] = 0;
$handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['body']['alter']['html'] = 0;
/* Argument: Node: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['validate_type'] = 'node';
$handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
  'blog' => 'blog',
  'group' => 0,
  'page' => 0,
  'story' => 0,
);
$handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
$handler->display->display_options['arguments']['nid']['break_phrase'] = 1;
$handler->display->display_options['arguments']['nid']['not'] = 0;

  $views[$view->name] = $view;

$view = new view;
$view->name = 'stories';
$view->description = 'New stories';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 3.0;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Historias Nuevas';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['use_more'] = TRUE;
$handler->display->display_options['use_more_always'] = TRUE;
$handler->display->display_options['use_more_text'] = 'More New Stories';
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '10';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Image (field_image) */
$handler->display->display_options['fields']['field_image_fid']['id'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['table'] = 'node_data_field_image';
$handler->display->display_options['fields']['field_image_fid']['field'] = 'field_image_fid';
$handler->display->display_options['fields']['field_image_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_image_fid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_image_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_image_fid']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_image_fid']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_image_fid']['link_to_node'] = 0;
$handler->display->display_options['fields']['field_image_fid']['label_type'] = 'none';
$handler->display->display_options['fields']['field_image_fid']['format'] = '100px_linked';
$handler->display->display_options['fields']['field_image_fid']['multiple']['group'] = 1;
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_number'] = '1';
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_from'] = '0';
$handler->display->display_options['fields']['field_image_fid']['multiple']['multiple_reversed'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'by';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = '';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
$handler->display->display_options['fields']['created']['date_format'] = 'custom';
$handler->display->display_options['fields']['created']['custom_date_format'] = 'n/j/Y - g:ia';
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Upload: Attached files */
$handler->display->display_options['fields']['upload_fid']['id'] = 'upload_fid';
$handler->display->display_options['fields']['upload_fid']['table'] = 'node';
$handler->display->display_options['fields']['upload_fid']['field'] = 'upload_fid';
$handler->display->display_options['fields']['upload_fid']['exclude'] = TRUE;
$handler->display->display_options['fields']['upload_fid']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['upload_fid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['upload_fid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['upload_fid']['alter']['html'] = 0;
$handler->display->display_options['fields']['upload_fid']['hide_empty'] = 0;
$handler->display->display_options['fields']['upload_fid']['empty_zero'] = 0;
$handler->display->display_options['fields']['upload_fid']['link_to_file'] = 0;
$handler->display->display_options['fields']['upload_fid']['only_listed'] = 0;
/* Field: Node: Body */
$handler->display->display_options['fields']['body']['id'] = 'body';
$handler->display->display_options['fields']['body']['table'] = 'node_revisions';
$handler->display->display_options['fields']['body']['field'] = 'body';
$handler->display->display_options['fields']['body']['label'] = '';
$handler->display->display_options['fields']['body']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['body']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['body']['alter']['trim'] = 1;
$handler->display->display_options['fields']['body']['alter']['max_length'] = '160';
$handler->display->display_options['fields']['body']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['body']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['body']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['body']['alter']['html'] = 0;
$handler->display->display_options['fields']['body']['hide_empty'] = 0;
$handler->display->display_options['fields']['body']['empty_zero'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'blog' => 'blog',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['path'] = 'stories';
$handler->display->display_options['menu']['title'] = 'Home';
$handler->display->display_options['menu']['description'] = 'Feature story';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'primary-links';

/* Display: Feed */
$handler = $view->new_display('feed', 'Feed', 'feed_1');
$handler->display->display_options['style_plugin'] = 'rss';
$handler->display->display_options['style_options']['mission_description'] = FALSE;
$handler->display->display_options['row_plugin'] = 'node_rss';
$handler->display->display_options['path'] = 'stories/feed';
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'block_1' => 'block_1',
  'default' => 0,
);

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['block_description'] = 'New stories';
$handler->display->display_options['block_caching'] = '8';

  $views[$view->name] = $view;

  return $views;
}
