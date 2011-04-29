<?php
// $Id: og_views.views_default.inc,v 1.3 2008/10/29 19:59:45 dww Exp $

function vozmob_support_views_default_views() {
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
$view->name = 'usuarios';
$view->description = 'vozmob bloggers';
$view->tag = 'bloggers';
$view->view_php = '';
$view->base_table = 'users';
$view->is_cacheable = FALSE;
$view->api_version = 3.0;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'grid';
$handler->display->display_options['style_options']['columns'] = '3';
$handler->display->display_options['style_options']['alignment'] = 'vertical';
$handler->display->display_options['style_options']['fill_single_line'] = 1;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['inline'] = array(
  'picture' => 'picture',
  'name' => 'name',
  'signature' => 'signature',
);
$handler->display->display_options['row_options']['separator'] = ' ';
$handler->display->display_options['row_options']['hide_empty'] = 0;
/* Field: User: Picture */
$handler->display->display_options['fields']['picture']['id'] = 'picture';
$handler->display->display_options['fields']['picture']['table'] = 'users';
$handler->display->display_options['fields']['picture']['field'] = 'picture';
$handler->display->display_options['fields']['picture']['label'] = '';
$handler->display->display_options['fields']['picture']['exclude'] = TRUE;
$handler->display->display_options['fields']['picture']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['picture']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['picture']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['picture']['alter']['trim'] = 0;
$handler->display->display_options['fields']['picture']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['picture']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['picture']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['picture']['alter']['html'] = 0;
$handler->display->display_options['fields']['picture']['hide_empty'] = 0;
$handler->display->display_options['fields']['picture']['empty_zero'] = 0;
$handler->display->display_options['fields']['picture']['imagecache_preset'] = 'sidebar';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['exclude'] = TRUE;
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 0;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: User: Signature */
$handler->display->display_options['fields']['signature']['id'] = 'signature';
$handler->display->display_options['fields']['signature']['table'] = 'users';
$handler->display->display_options['fields']['signature']['field'] = 'signature';
$handler->display->display_options['fields']['signature']['exclude'] = TRUE;
$handler->display->display_options['fields']['signature']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['signature']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['signature']['alter']['trim'] = 0;
$handler->display->display_options['fields']['signature']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['signature']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['signature']['alter']['html'] = 0;
/* Field: User: Uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'users';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = '';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['uid']['alter']['text'] = '[name]';
$handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['uid']['alter']['path'] = 'blog/[uid]';
$handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['uid']['alter']['html'] = 0;
$handler->display->display_options['fields']['uid']['hide_empty'] = 0;
$handler->display->display_options['fields']['uid']['empty_zero'] = 0;
$handler->display->display_options['fields']['uid']['link_to_user'] = 0;
/* Sort criterion: User: Last access */
$handler->display->display_options['sorts']['access']['id'] = 'access';
$handler->display->display_options['sorts']['access']['table'] = 'users';
$handler->display->display_options['sorts']['access']['field'] = 'access';
$handler->display->display_options['sorts']['access']['order'] = 'DESC';
/* Filter: User: Roles */
$handler->display->display_options['filters']['rid']['id'] = 'rid';
$handler->display->display_options['filters']['rid']['table'] = 'users_roles';
$handler->display->display_options['filters']['rid']['field'] = 'rid';
$handler->display->display_options['filters']['rid']['value'] = array(
  '3' => '3',
);
$handler->display->display_options['filters']['rid']['expose']['operator'] = FALSE;
/* Filter: User: Picture */
$handler->display->display_options['filters']['picture']['id'] = 'picture';
$handler->display->display_options['filters']['picture']['table'] = 'users';
$handler->display->display_options['filters']['picture']['field'] = 'picture';
$handler->display->display_options['filters']['picture']['value'] = '1';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Corresponsales / Reporters';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['exclude'] = TRUE;
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 0;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: User: Signature */
$handler->display->display_options['fields']['signature']['id'] = 'signature';
$handler->display->display_options['fields']['signature']['table'] = 'users';
$handler->display->display_options['fields']['signature']['field'] = 'signature';
$handler->display->display_options['fields']['signature']['exclude'] = TRUE;
$handler->display->display_options['fields']['signature']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['signature']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['signature']['alter']['trim'] = 0;
$handler->display->display_options['fields']['signature']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['signature']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['signature']['alter']['html'] = 0;
/* Field: User: Uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'users';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = '';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['uid']['alter']['text'] = '[name]';
$handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['uid']['alter']['path'] = 'blog/[uid]';
$handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['uid']['alter']['html'] = 0;
$handler->display->display_options['fields']['uid']['hide_empty'] = 0;
$handler->display->display_options['fields']['uid']['empty_zero'] = 0;
$handler->display->display_options['fields']['uid']['link_to_user'] = 0;
/* Field: User: Picture */
$handler->display->display_options['fields']['picture']['id'] = 'picture';
$handler->display->display_options['fields']['picture']['table'] = 'users';
$handler->display->display_options['fields']['picture']['field'] = 'picture';
$handler->display->display_options['fields']['picture']['label'] = '';
$handler->display->display_options['fields']['picture']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['picture']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['picture']['alter']['path'] = 'blog/[uid]';
$handler->display->display_options['fields']['picture']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['picture']['alter']['trim'] = 0;
$handler->display->display_options['fields']['picture']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['picture']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['picture']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['picture']['alter']['html'] = 0;
$handler->display->display_options['fields']['picture']['hide_empty'] = 0;
$handler->display->display_options['fields']['picture']['empty_zero'] = 0;
$handler->display->display_options['fields']['picture']['imagecache_preset'] = 'sidebar';
$handler->display->display_options['path'] = 'bloggers';

/* Display: Bloque */
$handler = $view->new_display('block', 'Bloque', 'block_1');
$handler->display->display_options['defaults']['use_more'] = FALSE;
$handler->display->display_options['use_more'] = TRUE;
$handler->display->display_options['use_more_text'] = 'Ver más corresponsales';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '3';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['inline'] = array(
  'picture' => 'picture',
  'name' => 'name',
  'signature' => 'signature',
);
$handler->display->display_options['row_options']['separator'] = ' ';
$handler->display->display_options['row_options']['hide_empty'] = 0;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: User: Picture */
$handler->display->display_options['fields']['picture']['id'] = 'picture';
$handler->display->display_options['fields']['picture']['table'] = 'users';
$handler->display->display_options['fields']['picture']['field'] = 'picture';
$handler->display->display_options['fields']['picture']['label'] = '';
$handler->display->display_options['fields']['picture']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['picture']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['picture']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['picture']['alter']['trim'] = 0;
$handler->display->display_options['fields']['picture']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['picture']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['picture']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['picture']['alter']['html'] = 0;
$handler->display->display_options['fields']['picture']['hide_empty'] = 0;
$handler->display->display_options['fields']['picture']['empty_zero'] = 0;
$handler->display->display_options['fields']['picture']['imagecache_preset'] = 'sidebar';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 0;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: User: Uid */
$handler->display->display_options['fields']['uid']['id'] = 'uid';
$handler->display->display_options['fields']['uid']['table'] = 'users';
$handler->display->display_options['fields']['uid']['field'] = 'uid';
$handler->display->display_options['fields']['uid']['label'] = '';
$handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
$handler->display->display_options['fields']['uid']['alter']['text'] = '- [name]\'s blog';
$handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
$handler->display->display_options['fields']['uid']['alter']['path'] = 'blog/[uid]';
$handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
$handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['uid']['alter']['html'] = 0;
$handler->display->display_options['fields']['uid']['hide_empty'] = 0;
$handler->display->display_options['fields']['uid']['empty_zero'] = 0;
$handler->display->display_options['fields']['uid']['link_to_user'] = 0;

  $views[$view->name] = $view;

  return $views;
}
