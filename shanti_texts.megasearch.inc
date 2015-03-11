<?php


    /* ADD TO MENU WHEN READY TO USE
    'shanti_texts_mega_search/node/%' => array(
    	'page callback' => 'shanti_texts_mega_search_node',
    	'page_arguments' => array(2),
	    'access callback' => 'user_access', 
  	  'access arguments' => array('access content'),
    	'type' => MENU_CALLBACK,
    ),
    */


function shanti_texts_mega_search_node($nid, $type = 'ajax') {
	$args = arg(); // Not sure why I have to do this -- passed argument does not work  
	$nid = $args[2];
	$node = node_load($nid);
  if ($type == 'ajax') {
    
		// Create the primary tabs
		$tabs_primary = array();
		$rs = db_query("select title, path, weight from menu_router where path rlike '^node/%/[^/]+$' and path not like '%delete%' order by weight desc");
    foreach ($rs as $r) { $tabs_primary[$r->path] = $r->title; }
		$tabs_primary_links = array();
		foreach ($tabs_primary as $tab_path => $tab_label) {
			$path = str_replace('%', $nid, $tab_path);
			if (drupal_valid_path($path,TRUE))  {
				$tabs_primary_links[] = l($tab_label,$path);
			}
		}
		$tabs_primary_ra = array(
			'#theme' 	     => 'item_list', 
			'#type' 	     => 'ul',
			'#attributes'  => array('class' => array('tabs','primary')),
			'#items' 	     => $tabs_primary_links,
		);
		$menu = '';
		if (count($tabs_primary_links) > 1) {
	  	$menu = drupal_render($tabs_primary_ra);
	  }

		// Create breadcrumbs
		global $base_url;
		$breadcrumbs_ra = array(
			'#theme' => 'item_list',
			'#type' => 'ol',
			'#attributes' => array('class' => array('breadcrumb')),
			'#items' => array(
				'<a href="'.$base_url.'">Texts:</a>',
				'<a href="/">Home</a> <i class="icon shanticon-arrow3-right"></i>',
				'<a href="#">'.$node->title.'</a> <i class="icon shanticon-arrow3-right"></i>'
			),
		);
		$breadcrumbs = drupal_render($breadcrumbs_ra);

		// Get the node content
		$html = drupal_render(node_view($node,'whole'));
  	
  	// Adjust and populate the page with jQuery
    $commands = array(
    	ajax_command_html('.region-content .content', $html),
    	ajax_command_invoke('#shanti-texts-toc', 'addClass', array('active')),
    	ajax_command_invoke('#shanti-texts-sidebar-tabs > li.first', 'addClass', array('active')),
    	ajax_command_remove("#node-$nid > h2"),
    	ajax_command_html('h1.page-title span.page-title-text', $node->title),
    	ajax_command_replace('.primary.tabs', $menu),
    	ajax_command_html('nav.breadwrap', $breadcrumbs),
    );
    $ajax = array('#type' => 'ajax', '#commands' => $commands);
    print ajax_deliver($ajax);
  }
  else {
  	drupal_goto("node/".$node->nid); // Actually, add frag and args here ...
  }	
}

function shanti_texts_mega_search_node($nid, $type = 'ajax') {
	$args = arg(); // Not sure why I have to do this -- passed argument does not work  
	$nid = $args[2];
	$node = node_load($nid);
  if ($type == 'ajax') {
    
		// Create the primary tabs
		$tabs_primary = array();
		$rs = db_query("select title, path, weight from menu_router where path rlike '^node/%/[^/]+$' and path not like '%delete%' order by weight desc");
    foreach ($rs as $r) { $tabs_primary[$r->path] = $r->title; }
		$tabs_primary_links = array();
		foreach ($tabs_primary as $tab_path => $tab_label) {
			$path = str_replace('%', $nid, $tab_path);
			if (drupal_valid_path($path,TRUE))  {
				$tabs_primary_links[] = l($tab_label,$path);
			}
		}
		$tabs_primary_ra = array(
			'#theme' 	     => 'item_list', 
			'#type' 	     => 'ul',
			'#attributes'  => array('class' => array('tabs','primary')),
			'#items' 	     => $tabs_primary_links,
		);
		$menu = '';
		if (count($tabs_primary_links) > 1) {
	  	$menu = drupal_render($tabs_primary_ra);
	  }

		// Create breadcrumbs
		global $base_url;
		$breadcrumbs_ra = array(
			'#theme' => 'item_list',
			'#type' => 'ol',
			'#attributes' => array('class' => array('breadcrumb')),
			'#items' => array(
				'<a href="'.$base_url.'">Texts:</a>',
				'<a href="/">Home</a> <i class="icon shanticon-arrow3-right"></i>',
				'<a href="#">'.$node->title.'</a> <i class="icon shanticon-arrow3-right"></i>'
			),
		);
		$breadcrumbs = drupal_render($breadcrumbs_ra);

		// Get the node content
		$html = drupal_render(node_view($node,'whole'));
  	
  	// Adjust and populate the page with jQuery
    $commands = array(
    	ajax_command_html('.region-content .content', $html),
    	ajax_command_invoke('#shanti-texts-toc', 'addClass', array('active')),
    	ajax_command_invoke('#shanti-texts-sidebar-tabs > li.first', 'addClass', array('active')),
    	ajax_command_remove("#node-$nid > h2"),
    	ajax_command_html('h1.page-title span.page-title-text', $node->title),
    	ajax_command_replace('.primary.tabs', $menu),
    	ajax_command_html('nav.breadwrap', $breadcrumbs),
    );
    $ajax = array('#type' => 'ajax', '#commands' => $commands);
    print ajax_deliver($ajax);
  }
  else {
  	drupal_goto("node/".$node->nid); // Actually, add frag and args here ...
  }	
}

function shanti_texts_block_info() {
  $blocks = array();
  $blocks['mega_search'] = array(
    'info' 	=> t('SHANTI Texts Mega Search'),
    'cache' => DRUPAL_NO_CACHE,
  );
  return $blocks;
}

function shanti_texts_block_view($delta = '') {
  $block = array();
  if ($delta == 'mega_search') {
    $block['subject'] = t('Full Text Search');
    if (user_access('access content')) {
      $block['content']['mega_search_form'] = drupal_get_form('shanti_texts_mega_search_form');
    }   
  } 
  return $block;
}

function shanti_texts_mega_search_form($form, &$form_state) {
	$form = array();
	$form['#attached'] = array('css' => array(SHANTI_TEXTS_PATH . '/css/shanti_texts_mega_search.css'));	
	$form['#tree'] = TRUE;
	$form['mega_search_string'] = array(
		'#type'       => 'textfield',
		'#size'       => 60,
		'#maxlength'  => 128,
		'#required'   => TRUE, 
		'#description' => t('Enter text to search for and press enter'),
		'#ajax' 	=> array(
			'callback' 	=> 'shanti_texts_mega_search_form_callback',
			'wrapper' 	=> 'shanti-texts-mega-search-results',
			'method' 		=> 'replace',
			'effect' 		=> 'fade',
			'event'			=> 'change',
			'keypress'	=> TRUE,
		),
	);

	#$form['mega_search_type'] = array(
	#	'#title' 					=> t('Search type'),
	#	'#type'						=> 'radios',
	#	'#states'         => array(),
	#	'#options' 				=> array(0 => t('Title'), 1 => t('Full Text')),
  #  '#default_value' 	=> isset($form_state['values']['mega_search_type']) ? $form_state['values']['mega_search_type'] : 0,
	#);

	$form['mega_search_submit'] = array(
		'#type' 			=> 'submit',
		'#value' 			=> 'Search',
		'#attributes' => array('class' => array('btn')),
		'#ajax' 			=> array(
			'callback' 		=> 'shanti_texts_mega_search_form_callback',
			'wrapper' 		=> 'shanti-texts-mega-search-results',
			'method' 			=> 'replace',
			'effect' 			=> 'fade',
			'event'				=> 'change',
		),
	);
	$form['mega_search_results'] = array(
		'#theme'      => 'item_list',
		'#type'       => 'ol',
		'#prefix'			=> "<div id='shanti-texts-mega-search-results'>",
		'#suffix'			=> "</div>",
		//'#items'			=> isset($form_state['values']['mega_search_string']) ? _shanti_texts_mega_search_query($form_state['values']['mega_search_string'],$form_state['values']['mega_search_type']) : _shanti_texts_mega_search_query(''),
		'#items'			=> isset($form_state['values']['mega_search_string']) ? _shanti_texts_mega_search_query($form_state['values']['mega_search_string'],1) : _shanti_texts_mega_search_query(''),
	);
	return $form;
}

function shanti_texts_mega_search_form_validate($form, &$form_state) {
	if (preg_match("/^\s*$/", $form_state['values']['mega_search_string'])) {
		form_set_error('mega_search_string', t('Enter a search string.'));
	}
}

// DEPENDS ON FIELD NAME -- although if BODY were used, this would not be a prob
function _shanti_texts_mega_search_query($search_str = '', $search_type = 1, $limit_offset = 0) {
  $items = array();
  $limit_length = 50; # May want to pull this out and set it somewhere
	if ($search_str && $search_type == 1) {
		$pattern = "([.?!;,\]\)]|\"|')\s+"; // To split textareas into sentences
		$sql = "SELECT b.bid, n.nid, p.title as 'parent', n.title"
			. ", c.field_book_content_value as 'content' "
			. "FROM {field_data_field_book_content} c "
			. "JOIN {node} n ON (c.entity_id = n.nid) "
			. "JOIN {book} b ON (b.nid = n.nid) "
			. "JOIN {node} p ON (b.bid = p.nid) "
			. "JOIN {menu_links} m ON (b.mlid = m.mlid) "
			. "WHERE {field_book_content_value} RLIKE :search_str "
			. "ORDER BY b.bid, m.depth, m.weight "
			. "LIMIT $limit_offset, $limit_length "
			; // A badass SQL statement
		$rs = db_query($sql, array(':search_str' => $search_str));
		if ($rs) {
			$k = array(); // Keeps a hit count per book, to properly identify kwic hits
			$viewable = array();
			if ($rs->rowCount()) {
				foreach($rs as $r) {
					$this_node = node_load($r->nid); // Trap this?
					if (!isset($viewable[$r->nid])) {
						$viewable[$r->nid] = node_access('view', $this_node);
					} 
					$hits = 0;
					if ($viewable[$r->nid]) { // Check if node is viewable
						$hits++;
						$k[$r->bid] = isset($k[$r->bid]) ? $k[$r->bid] : 0;
						$content = strip_tags($r->content);
						$v = field_view_field('node',$this_node,'field_book_content');
						$content_array = preg_split("/$pattern/i", $content);
						$kwic_array = array();
						foreach($content_array as $n => $line) {
							if (preg_match("/$search_str/i",$line)) {
								$k[$r->bid]++; // We count number of hits per book, not per node, to match hit count on book viewer page
								$href = url("shanti_texts_mega_search/node/".$r->bid.'/nojs/', array('fragment' => 'book-node-' . $r->nid, 'query' => array('s' => $search_str, 'n' => $k[$r->bid])));
								$kwic_array[] = preg_replace("/($search_str)/i","<a href='$href' class='use-ajax shanti-texts-search-hit'>$1</a>",$line);
							}
						}
						$kwic_count = count($kwic_array);
						$kwic = "<div class='shanti-texts-mega-search-kwic'>".implode("</div><div class='shanti-texts-mega-search-kwic'>",$kwic_array)."</div>";
						$items[] = l($r->parent .': '. $r->title, "shanti_texts_mega_search/node/".$r->bid.'/nojs/', array('fragment' => 'book-node-' . $r->nid, 'query' => array('s' => $search_str), 'attributes' => array('class' => array('use-ajax'))))  
						. "<div>$kwic_count result(s):</div>"
						. "<div id='kwic-hits-{$r->nid}'>$kwic</div>"; 					
					} 
					else {
						drupal_set_message('Content restricted for node ' . $r->nid);
					}
				} 	
			} 
			else {
				drupal_set_message("Got no results for search string <tt>$search_str</tt>.");	
			}	
		} 
		else {
			drupal_set_message("Bad things must have happened.");	
		}	
	} 
	elseif ($search_str && $search_type == 0) {
		$sql = "SELECT n.nid, n.title FROM {node} n "
		. "JOIN {book} b USING(nid) "
		. "WHERE b.nid = b.bid AND n.title RLIKE :search_str "
		. "ORDER BY n.title "
		. "LIMIT $limit_offset, $limit_length "
		;
		$rs = db_query($sql, array(':search_str' => $search_str));
		if ($rs) {
			$viewable = array();
			if ($rs->rowCount()) {
				foreach($rs as $r) {
					$this_node = node_load($r->nid); // Trap this?
					if (!isset($viewable[$r->nid])) {
						$viewable[$r->nid] = node_access('view', $this_node);
					} 
					$hits = 0;
					if ($viewable[$r->nid]) { // Check if node is viewable
						$hits++;
						$k[$r->bid] = isset($k[$r->bid]) ? $k[$r->bid] : 0;
						$items[] = l(
							$r->title, "shanti_texts_mega_search/node/".$r->nid.'/nojs/', 
							array('attributes' => array('class' => array('use-ajax')))
						); 					
					} 
					else {
						drupal_set_message('Content restricted for node ' . $r->nid);
					}
				} 	
			} 
			else {
				drupal_set_message("Got no results.");	
			}	
		} 
	}
	elseif (!$search_str) {
	  $sql = "SELECT n.nid, n.title "
	  . "FROM {book} b JOIN {node} n USING(nid) "
	  . "WHERE b.nid = b.bid "
	  . "ORDER BY title "
	  . "LIMIT $limit_offset, $limit_length "
	  ;
	  $rs = db_query($sql);
	  if ($rs) {
	    foreach ($rs as $r) {
        $link = array(
          '#theme' => 'link',
          '#path' => 'shanti_texts_mega_search/node/'.$r->nid.'/nojs/',
          '#text' =>  $r->title, 
          '#options' => array(
            'attributes' => array('class' => array('use-ajax')),
            'html' => FALSE,
          ),
        );
        $items[] = drupal_render($link);	    
	    }
	  }

		#foreach(book_get_books() as $book) {
		#	$link = array(
		#		'#theme' => 'link',
		#		'#path' => 'shanti_texts_mega_search/'.$book['link_path'].'/nojs/',
		#		'#text' =>  $book['link_title'], 
		#		'#options' => array(
		#			'attributes' => array('class' => array('use-ajax')),
		#			'html' => FALSE,
		#		),
		#	);
		#	$items[] = drupal_render($link);
    #}

	}
	else {
		// Not sure how we get here
	}
	if ($search_str && count($items)) drupal_set_message(count($items) . " pages found with content matching <tt>$search_str</tt>");
	//$items[] = "<p>Pager goes here</p>";
	return $items;
}

function shanti_texts_mega_search_form_callback($form, $form_state) {
	return $form['mega_search_results'];
}