#ifndef NEARESTROADSWRITER_HPP_
#define NEARESTROADSWRITER_HPP_

class NearestRoadsWriter : public Writer {

public:

	NearestRoadsWriter(OGRDataSource* data_source) :
		Writer(data_source, "osmi_addresses_nearest_roads", USE_TRANSACTIONS, wkbLineString)  {

		std::vector<field_config> field_configurations;
		field_configurations.push_back({"way_id",     OFTString, NO_WIDTH});
		field_configurations.push_back({"name",       OFTString, NO_WIDTH});
		field_configurations.push_back({"lastchange", OFTString, NO_WIDTH});

		create_fields(field_configurations);
	}

	void write_road(
			const std::unique_ptr<OGRLineString>&  way,
			const osmium::unsigned_object_id_type& way_id,
			const char*                            addrstreet,
			const std::string&                     lastchange) {

		if (written_ways.find(way_id) == written_ways.end()) {
			OGRFeature* feature = OGRFeature::CreateFeature(m_layer->GetLayerDefn());
			feature->SetGeometry(way.get());
			feature->SetField("name", addrstreet);
			feature->SetField("way_id", static_cast<double>(way_id)); //TODO: closest_way_id is of type int64_t. is this ok?
			feature->SetField("lastchange", lastchange.c_str());

			create_feature(feature);

			written_ways.emplace(way_id);
		}
	}

	void feed_node(const osmium::Node& /* node */) {

	}

	void feed_way(const osmium::Way& /* way */) {

	}

	void feed_relation(const osmium::Relation& /* relation */) {

	}


private:
	std::unordered_set<osmium::unsigned_object_id_type> written_ways;
};



#endif /* NEARESTROADSWRITER_HPP_ */
