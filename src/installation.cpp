#include <installation.hpp>

#include <util-generic/executable_path.hpp>

#include <filesystem>
//#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <stdexcept>

namespace singular_buchberger
{
  namespace
  {
    void check ( std::filesystem::path const& path
               , bool okay
               , std::string const& message
               )
    {
      if (!okay)
      {
        throw std::logic_error
          ( ( boost::format ("%1% %2%: Installation incomplete!?")
            % path
            % message
            ).str()
          );
      }
    }

    void check_is_directory (std::filesystem::path const& path)
    {
      check ( path
            , std::filesystem::is_directory (path)
            , "is not a directory"
            );
    }
    void check_is_file (std::filesystem::path const& path)
    {
      check ( path
            , std::filesystem::exists (path)
            , "does not exist"
            );
      check ( path
            , std::filesystem::is_regular_file (path)
            , "is not a regular file"
            );
    }

    //! \todo configure
    std::filesystem::path gspc_home
      (std::filesystem::path const& gspc_path)
    {
      return gspc_path;
    }
    std::filesystem::path workflow_path
      (std::filesystem::path const& installation_path)
    {
      return installation_path / "libexec" / "workflow";
    }
    std::filesystem::path workflow_all_file
      (std::filesystem::path const& installation_path)
    {
      return workflow_path (installation_path) / "buchberger.pnet";
    }
  }

  installation::installation()
    : installation
        (std::filesystem::path(SP_INSTALL_PATH))
  {}

  installation::installation (boost::filesystem::path const& ip)
    : installation
        (std::filesystem::path(ip.string()))
  {}

  installation::installation (std::filesystem::path const& ip)
    : installation ( ip
                   , std::filesystem::path (fhg::util::executable_path
                       (
                         static_cast<void(*)(boost::program_options::variables_map&, const std::filesystem::path&)>(gspc::set_gspc_home)
                         //static_cast<void(*)(boost::program_options::variables_map&, const std::filesystem::path&)>(gspc::set_gspc_home) // deprecated
                         //gspc::set_gspc_home
                       ).parent_path().parent_path().string())
                   )
  {}

  installation::installation (std::filesystem::path const& ip,
    std::filesystem::path const& gp)
    : _path (ip), _gspc_path (gp)
  {
    //! \todo more detailed tests!?
    check_is_directory (gspc_home (_gspc_path));
    check_is_directory (workflow_path (_path));
    check_is_file (workflow_all());
  }

  std::filesystem::path installation::workflow_all() const
  {
    return workflow_all_file (_path);
  }
  std::filesystem::path installation::workflow_dir() const
  {
    return workflow_path (_path);
  }
  gspc::installation installation::gspc_installation
    (boost::program_options::variables_map& vm) const
  {
    gspc::set_gspc_home (vm, gspc_home (_gspc_path));
    gspc::set_application_search_path (vm, workflow_path (_path));

    return {vm};
  }
}
