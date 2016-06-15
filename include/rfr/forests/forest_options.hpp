#ifndef RFR_FOREST_OPTIONS_HPP
#define RFR_FOREST_OPTIONS_HPP


#include "cereal/cereal.hpp"


#include "rfr/data_containers/data_container_base.hpp"
#include "rfr/trees/tree_options.hpp"


namespace rfr{ namespace forests{

template <typename num_type = float,typename response_type = float, typename index_type = unsigned int>
struct forest_options{
	index_type num_trees;
	index_type num_data_points_per_tree;

	bool do_bootstrapping;
	bool compute_oob_error;
	rfr::trees::tree_options<num_type,response_type,index_type> tree_opts;


  	/* serialize function for saving forests */
  	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( num_trees,num_data_points_per_tree, do_bootstrapping, tree_opts);
	}


	/** (Re)set to default values for the forest.
	 * 
	 * 
	 */
	void set_default_values(){
		num_trees = 10;
		num_data_points_per_tree = 0;

		do_bootstrapping = true;
		compute_oob_error = false;
		
		tree_opts.set_default_values();
	}


	/** adjusts all relevant variables to the data */
	void adjust_limits_to_data (const rfr::data_containers::data_container_base<num_type, response_type, index_type> &data){
		num_data_points_per_tree = data.num_data_points();
	}

	/** Default constructor that initializes the values with their default */
	forest_options(){ set_default_values();}

	/** Constructor to feed in tree values but leave the forest parameters at their default.*/
	forest_options(rfr::trees::tree_options<num_type,response_type,index_type> & to): tree_opts(to) { set_default_values();}

	/** Constructor that adjusts to the data. */   
	forest_options (rfr::trees::tree_options<num_type,response_type,index_type> & to, rfr::data_containers::data_container_base<num_type, response_type, index_type> &data): tree_opts(to){
		set_default_values();
		adjust_limits_to_data(data);
	}
};

}}//namespace rfr::forests
#endif
