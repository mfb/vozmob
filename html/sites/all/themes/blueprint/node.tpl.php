<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block">

  <?php if ($page == 0): ?>
    <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>

  <?php if ($submitted || $terms): ?>
    <div class="meta">
      <?php if ($submitted): ?>
        <div class="submitted"><?php print $submitted ?></div>
      <?php endif; ?>

      <?php if ($terms): ?>
        <div class="terms"><?php print $terms ?></div>
      <?php endif;?>
    </div>
  <?php endif; ?>

  <div class="content2 clear-block">
    <?php if ($page): ?>
      <?php print $picture; ?>
    <?php endif; ?>

    <?php if ($overlay_launcher): ?>
      <div class="overlay-launcher">
        <a href="<?php print $node_url ?>"><?php print theme('image', $overlay_launcher_image, t('Launch presentation.'), t('Launch presentation.')); ?></a>
      </div>
    <?php endif; ?>

    <?php print $content ?>
    <?php print views_embed_view('overlay', 'default', $node->nid); ?>
  </div>

  <?php if (!empty($node->og_groups) && $page): ?>
    <div class="groups">
      <?php print t('Groups') . ': '; ?>
      <div class="links"><?php print $og_links['view']; ?></div>
    </div>
  <?php endif; ?>

  <?php if ($links): ?>
    <div class="node-links">
      <?php if (!$page): ?>
        <?php print $picture; ?>
      <?php endif; ?>
      <?php print $links; ?>
    </div>
  <?php endif; ?>

</div>
