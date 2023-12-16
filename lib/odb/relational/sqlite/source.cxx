// file      : odb/relational/sqlite/source.cxx
// copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
// license   : GNU GPL v3; see accompanying LICENSE file

#include <odb/relational/source.hxx>

#include <odb/relational/sqlite/common.hxx>
#include <odb/relational/sqlite/context.hxx>

using namespace std;

namespace relational
{
  namespace sqlite
  {
    namespace source
    {
      namespace relational = relational::source;

      //
      // bind
      //

      struct bind_member: relational::bind_member_impl<sql_type>,
                          member_base
      {
        bind_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << b << ".type = sqlite::bind::integer;"
             << b << ".buffer = &" << arg << "." << mi.var << "value;"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << b << ".type = sqlite::bind::real;"
             << b << ".buffer = &" << arg << "." << mi.var << "value;"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_text (member_info& mi)
        {
          os << b << ".type = sqlite::image_traits<" << endl
             << "  " << mi.fq_type () << "," << endl
             << "  sqlite::id_text>::bind_value;"
             << b << ".buffer = " << arg << "." << mi.var << "value.data ();"
             << b << ".size = &" << arg << "." << mi.var << "size;"
             << b << ".capacity = " << arg << "." << mi.var <<
            "value.capacity ();"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }

        virtual void
        traverse_blob (member_info& mi)
        {
          os << b << ".type = sqlite::bind::blob;"
             << b << ".buffer = " << arg << "." << mi.var << "value.data ();"
             << b << ".size = &" << arg << "." << mi.var << "size;"
             << b << ".capacity = " << arg << "." << mi.var <<
            "value.capacity ();"
             << b << ".is_null = &" << arg << "." << mi.var << "null;";
        }
      };
      entry<bind_member> bind_member_;

      //
      // grow
      //

      struct grow_member: relational::grow_member_impl<sql_type>,
                          member_base
      {
        grow_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x) {}

        virtual void
        traverse_integer (member_info&)
        {
          os << e << " = false;"
             << endl;
        }

        virtual void
        traverse_real (member_info&)
        {
          os << e << " = false;"
             << endl;
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << "if (" << e << ")" << endl
             << "{"
             << "i." << mi.var << "value.capacity (i." << mi.var << "size);"
             << "grew = true;"
             << "}";
        }
      };
      entry<grow_member> grow_member_;

      //
      // init image
      //

      struct init_image_member: relational::init_image_member_impl<sql_type>,
                                member_base
      {
        init_image_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        set_null (member_info& mi)
        {
          os << "i." << mi.var << "null = true;";
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;";
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;";
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << "std::size_t cap (i." << mi.var << "value.capacity ());"
             << traits << "::set_image (" << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "size," << endl
             << "is_null," << endl
             << member << ");"
             << "i." << mi.var << "null = is_null;"
             << "grew = grew || (cap != i." << mi.var << "value.capacity ());";
        }
      };
      entry<init_image_member> init_image_member_;

      //
      // init value
      //

      struct init_value_member: relational::init_value_member_impl<sql_type>,
                                member_base
      {
        init_value_member (base const& x)
            : member_base::base (x),      // virtual base
              member_base::base_impl (x), // virtual base
              base_impl (x),
              member_base (x)
        {
        }

        virtual void
        get_null (string const& var) const
        {
          os << "i." << var << "null";
        }

        virtual void
        traverse_integer (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "null);"
             << endl;
        }

        virtual void
        traverse_real (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "null);"
             << endl;
        }

        virtual void
        traverse_string (member_info& mi)
        {
          os << traits << "::set_value (" << endl
             << member << "," << endl
             << "i." << mi.var << "value," << endl
             << "i." << mi.var << "size," << endl
             << "i." << mi.var << "null);"
             << endl;
        }
      };
      entry<init_value_member> init_value_member_;

      struct container_traits: relational::container_traits, context
      {
        container_traits (base const& x): base (x) {}

        virtual void
        cache_result (string const&)
        {
          // Caching is not necessary since SQLite can execute several
          // interleaving statements.
          //
        }
      };
      entry<container_traits> container_traits_;

      struct class_: relational::class_, context
      {
        class_ (base const& x): base (x) {}

        virtual void
        init_auto_id (semantics::data_member&, string const& im)
        {
          os << im << "null = true;";
        }

        virtual string
        select_trailer (type&)
        {
          // SQLite has not support for FOR UPDATE and since this is an
          // optimization, we simply ignore it.
          //
          return "";
        }

        virtual string
        join_syntax (view_object const& vo)
        {
          const char* n (0);

          if (vo.join == view_object::full)
            n = "FULL OUTER JOIN";
          else if (vo.join == view_object::right)
            n = "RIGHT OUTER JOIN";

          if (n != 0)
          {
            error (vo.loc) << n << " is not supported by SQLite" << endl;
            throw operation_failed ();
          }

          return base::join_syntax (vo);
        }
      };
      entry<class_> class_entry_;
    }
  }
}
