<?php
// $Id: FeedsOPMLParser.inc,v 1.6 2010/09/07 17:29:36 alexb Exp $

/**
 * @file
 * OPML Parser plugin.
 */

/**
 * Feeds parser plugin that parses OPML feeds.
 */
class FeedsOPMLParser extends FeedsParser {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    feeds_include_library('opml_parser.inc', 'opml_parser');
    $result = opml_parser_parse($batch->getRaw());
    $batch->setTitle($result['title']);
    $batch->setItems($result['items']);
  }

  /**
   * Return mapping sources.
   */
  public function getMappingSources() {
    return array(
      'title' => array(
        'name' => t('Feed title'),
        'description' => t('Title of the feed.'),
      ),
      'xmlurl' => array(
        'name' => t('Feed URL'),
        'description' => t('URL of the feed.'),
      ),
    ) + parent::getMappingSources();
  }
}
