(function($){

Drupal.behaviors.shantiTexts = {
  
  attach: function (context, settings) {   
  
    var kwic_n = Drupal.settings.shantiTexts.kwic_n;
    if (kwic_n) {
      // If there is a kwic number ...
      location.href = '#' + 'shanti-texts-search-hit-' + kwic_n;
    } else {
      // This handles scrolling to anchors which doesn't otherwise work  
      location.href = '#' + window.location.hash.substr(1);
    }
    
    $('#shanti-texts-sidebar-tabs li.first a').tab('show'); // Bootstrap
    
    //$('#shanti-texts-sidebar').css('display','block');   

    $('.toc-item-node', context).localScroll({
      target:'#shanti-texts-body',
      duration: 10,
    });
    
  },

  detach: function (context, settings) {
  
  },

};
  
})(jQuery);