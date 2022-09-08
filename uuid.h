#include <iostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

std::string gen_uuid()
{
    boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
    return boost::uuids::to_string(a_uuid);
}
