# file      : odb/Makefile.am
# copyright : Copyright (c) 2009-2015 Code Synthesis Tools CC
# license   : GNU GPL v3; see accompanying LICENSE file

plugindir = @plugindir@

bin_PROGRAMS = odb
plugin_LTLIBRARIES = odb.la

AM_CPPFLAGS = -I'$(top_builddir)' -I'$(top_srcdir)'

EXTRA_DIST = common-query.hxx common.hxx context.hxx context.ixx cxx-lexer.hxx cxx-token.hxx diagnostics.hxx emitter.hxx features.hxx gcc-fwd.hxx gcc.hxx generate.hxx generator.hxx instance.hxx location.hxx lookup.hxx option-functions.hxx option-parsers.hxx option-types.hxx options.hxx options.ixx parser.hxx pragma.hxx processor.hxx profile.hxx relational/common-query.hxx relational/common.hxx relational/common.txx relational/context.hxx relational/context.ixx relational/generate.hxx relational/header.hxx relational/inline.hxx relational/model.hxx relational/mssql/common.hxx relational/mssql/context.hxx relational/mysql/common.hxx relational/mysql/context.hxx relational/oracle/common.hxx relational/oracle/context.hxx relational/pgsql/common.hxx relational/pgsql/context.hxx relational/processor.hxx relational/schema-source.hxx relational/schema.hxx relational/source.hxx relational/sqlite/common.hxx relational/sqlite/context.hxx relational/validator.hxx semantics.hxx semantics/class-template.hxx semantics/class.hxx semantics/derived.hxx semantics/elements.hxx semantics/elements.ixx semantics/enum.hxx semantics/fundamental.hxx semantics/namespace.hxx semantics/relational.hxx semantics/relational/changelog.hxx semantics/relational/changeset.hxx semantics/relational/column.hxx semantics/relational/deferrable.hxx semantics/relational/elements.hxx semantics/relational/elements.txx semantics/relational/foreign-key.hxx semantics/relational/index.hxx semantics/relational/key.hxx semantics/relational/model.hxx semantics/relational/name.hxx semantics/relational/primary-key.hxx semantics/relational/table.hxx semantics/template.hxx semantics/union-template.hxx semantics/union.hxx semantics/unit.hxx sql-lexer.hxx sql-lexer.ixx sql-token.hxx sql-token.ixx traversal.hxx traversal/class-template.hxx traversal/class.hxx traversal/derived.hxx traversal/elements.hxx traversal/enum.hxx traversal/fundamental.hxx traversal/namespace.hxx traversal/relational.hxx traversal/relational/changelog.hxx traversal/relational/changeset.hxx traversal/relational/column.hxx traversal/relational/elements.hxx traversal/relational/foreign-key.hxx traversal/relational/index.hxx traversal/relational/key.hxx traversal/relational/model.hxx traversal/relational/primary-key.hxx traversal/relational/table.hxx traversal/template.hxx traversal/union-template.hxx traversal/union.hxx traversal/unit.hxx validator.hxx version.hxx options.cli

# Plugin.
#
odb_la_SOURCES = cxx-lexer.cxx sql-token.cxx sql-lexer.cxx context.cxx common.cxx common-query.cxx location.cxx diagnostics.cxx emitter.cxx lookup.cxx instance.cxx include.cxx header.cxx inline.cxx source.cxx validator.cxx processor.cxx generator.cxx parser.cxx plugin.cxx pragma.cxx relational/changelog.cxx relational/common.cxx relational/common-query.cxx relational/context.cxx relational/processor.cxx relational/header.cxx relational/inline.cxx relational/source.cxx relational/model.cxx relational/schema.cxx relational/schema-source.cxx relational/validator.cxx relational/mssql/common.cxx relational/mssql/context.cxx relational/mssql/header.cxx relational/mssql/inline.cxx relational/mssql/source.cxx relational/mssql/model.cxx relational/mssql/schema.cxx relational/mysql/common.cxx relational/mysql/context.cxx relational/mysql/header.cxx relational/mysql/inline.cxx relational/mysql/source.cxx relational/mysql/model.cxx relational/mysql/schema.cxx relational/oracle/common.cxx relational/oracle/context.cxx relational/oracle/header.cxx relational/oracle/inline.cxx relational/oracle/source.cxx relational/oracle/model.cxx relational/oracle/schema.cxx relational/pgsql/common.cxx relational/pgsql/context.cxx relational/pgsql/header.cxx relational/pgsql/inline.cxx relational/pgsql/source.cxx relational/pgsql/model.cxx relational/pgsql/schema.cxx relational/sqlite/common.cxx relational/sqlite/context.cxx relational/sqlite/header.cxx relational/sqlite/inline.cxx relational/sqlite/source.cxx relational/sqlite/model.cxx relational/sqlite/schema.cxx semantics/class.cxx semantics/class-template.cxx semantics/derived.cxx semantics/elements.cxx semantics/enum.cxx semantics/fundamental.cxx semantics/namespace.cxx semantics/template.cxx semantics/union.cxx semantics/union-template.cxx semantics/unit.cxx semantics/relational/changelog.cxx semantics/relational/changeset.cxx semantics/relational/column.cxx semantics/relational/elements.cxx semantics/relational/foreign-key.cxx semantics/relational/index.cxx semantics/relational/key.cxx semantics/relational/model.cxx semantics/relational/primary-key.cxx semantics/relational/table.cxx traversal/class.cxx traversal/class-template.cxx traversal/derived.cxx traversal/elements.cxx traversal/enum.cxx traversal/template.cxx traversal/union-template.cxx traversal/relational/changelog.cxx traversal/relational/key.cxx option-types.cxx option-functions.cxx profile.cxx semantics/relational/name.cxx semantics/relational/deferrable.cxx options.cxx
odb_la_LDFLAGS = -module -shrext .so -avoid-version

# Remove the .la file from the final install.
#
install-data-hook:
	rm -f '$(DESTDIR)$(plugindir)/odb.la'

# Driver.
#
odb_SOURCES = odb.cxx option-types.cxx option-functions.cxx profile.cxx semantics/relational/name.cxx semantics/relational/deferrable.cxx options.cxx

# Make sure common sources are compiled differently.
#
odb_CXXFLAGS = $(AM_CXXFLAGS)
