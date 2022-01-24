/**
 * @file hpecodesdialogform.cpp
 * @brief This file is part of HPEWidgets
 * @version 1.0.0
 * @date 2022-01-21
 * 
 * @author Tomortec (everything@tomortec.com)
 * @copyright Copyright © 2021 - 2022 Tomortec.
 * @website https://tomortec.com
 * @license GPL v3 (https://www.gnu.org/licenses/gpl-3.0.html)
*/

#include "hpecodesdialogform.h"
#include "ui_hpecodesdialogform.h"

#include <QWebChannel>
#include <QLineEdit>
#include <QCompleter>

#include "Controller/hpepreviewpage.h"

HPECodesDialogForm::HPECodesDialogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HPECodesDialogForm)
{
    ui->setupUi(this);

    ui->languageSelector->addItems(HighlightJSLanguages::HighlightJSSupportedLanguages);
    ui->languageSelector->lineEdit()->setPlaceholderText(tr("Select Language"));
    ui->languageSelector->setCurrentIndex(-1);
    QCompleter* completer = new QCompleter(HighlightJSLanguages::HighlightJSSupportedLanguages, ui->languageSelector);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->languageSelector->setCompleter(completer);

    HPEPreviewPage* page = new HPEPreviewPage(this);
    ui->codesWidget->setPage(page);
    ui->codesWidget->setUrl(QUrl("qrc:/resources/codeDialog/codesRendering.html"));
    ui->codesWidget->setContextMenuPolicy(Qt::NoContextMenu);

    QWebChannel* channel = new QWebChannel(this);
    HPECodesObject* codesObj = new HPECodesObject(this);
    channel->registerObject("codeObj", codesObj);
    ui->codesWidget->page()->setWebChannel(channel);

    connect(codesObj, &HPECodesObject::sentCodes, this, &HPECodesDialogForm::receiveCodes);
    connect(this, &HPECodesDialogForm::resetCodesEdit, codesObj, &HPECodesObject::resetCodesEdit);
    connect(ui->languageSelector, &QComboBox::currentTextChanged, this, &HPECodesDialogForm::changeLanguage);
    connect(this, &HPECodesDialogForm::languageChanged, codesObj, &HPECodesObject::changeCodesLanguage);
}

HPECodesDialogForm::~HPECodesDialogForm()
{
    delete ui;
}

QString HPECodesDialogForm::getCodes() const
{
    return m_codes;
}

QString HPECodesDialogForm::getLanguage() const
{
    return m_language;
}

void HPECodesDialogForm::resetEdit()
{
    m_codes = "";
    m_language = "";
    emit resetCodesEdit();
}

void HPECodesDialogForm::receiveCodes(const QString &codes)
{
    m_codes = codes;
}

void HPECodesDialogForm::changeLanguage(const QString &lang)
{
    m_language = lang;
    emit languageChanged(lang);
}

void HPECodesObject::getCodesFromWebPage(const QString &codes)
{
    emit sentCodes(codes);
}

void HPECodesObject::changeCodesLanguage(const QString &lang)
{
    emit changeLanguage(lang);
}

void HPECodesObject::resetCodesEdit()
{
    emit resetEdit();
}

const QStringList HighlightJSLanguages::HighlightJSSupportedLanguages = {
    "1C", "4D", "ABAP", "ABNF", "Access logs", "Ada", "Arduino (C++ w/Arduino libs)", "ARM assembler", "AVR assembler", "ActionScript",
    "Alan IF", "Alan", "AngelScript", "Apache", "AppleScript", "Arcade", "AsciiDoc", "AspectJ", "AutoHotkey", "AutoIt",
    "Awk", "Bash", "Basic", "BBCode", "Blade (Laravel)", "BNF", "Brainfuck", "C#", "C", "C++",
    "C/AL", "Cache Object Script", "CMake", "Coq", "CSP", "CSS", "Cap’n Proto", "Chaos", "Chapel", "Cisco CLI",
    "Clojure", "CoffeeScript", "CpcdosC+", "Crmsh", "Crystal", "Cypher (Neo4j)", "D", "Dafny", "Dart", "Delphi",
    "Diff", "Django", "DNS Zone file", "Dockerfile", "DOS", "dsconfig", "DTS (Device Tree)", "Dust", "Dylan", "EBNF",
    "Elixir", "Elm", "Erlang", "Excel", "Extempore", "F#", "FIX", "Fortran", "G-Code", "Gams",
    "GAUSS", "GDScript", "Gherkin", "Glimmer and EmberJS", "GN for Ninja", "Go", "Grammatical Framework", "Golo", "Gradle", "Groovy",
    "GSQL", "HTML", "XML", "HTTP", "Haml", "Handlebars", "Haskell", "Haxe", "High-level shader language", "Hy", "Ini, TOML",
    "Inform7", "IRPF90", "JSON", "Java", "JavaScript", "Jolie", "Julia", "Kotlin", "LaTeX", "Leaf",
    "Lean", "Lasso", "Less", "LDIF", "Lisp", "LiveCode Server", "LiveScript", "Lua", "Makefile", "Markdown",
    "Mathematica", "Matlab", "Maxima", "Maya Embedded Language", "Mercury", "mIRC Scripting Language", "Mizar", "MKB", "Mojolicious", "Monkey",
    "Moonscript", "N1QL", "NSIS", "Never", "Nginx", "Nim", "Nix", "Object Constraint Language", "OCaml", "Objective C",
    "OpenGL Shading Language", "OpenSCAD", "Oracle Rules Language", "Oxygene", "PF", "PHP", "Papyrus", "Parser3", "Perl", "Pine Script",
    "Plaintext", "Pony", "PostgreSQL & PL/pgSQL", "PowerShell", "Processing", "Prolog", "Properties", "Protocol Buffers", "Puppet", "Python",
    "Python profiler results", "Python REPL", "Q#", "Q", "QML", "R", "Razor CSHTML", "ReasonML", "Rebol & Red", "RenderMan RIB",
    "RenderMan RSL", "RiScript", "Roboconf", "Robot Framework", "RPM spec files", "Ruby", "Rust", "SAS", "SCSS", "SQL",
    "STEP Part 21", "Scala", "Scheme", "Scilab", "Shape Expressions", "Shell", "Smali", "Smalltalk", "SML", "Solidity",
    "Splunk SPL", "Stan", "Stata", "Structured Text", "Stylus", "SubUnit", "Supercollider", "Svelte", "Swift", "Tcl",
    "Terraform (HCL)", "Test Anything Protocol", "Thrift", "TP", "Transact-SQL", "Twig", "TypeScript", "Unicorn Rails log", "VB.Net", "VBA",
    "VBScript", "VHDL", "Vala", "Verilog", "Vim Script", "X#", "X++", "x86 Assembly", "XL", "XQuery",
    "YAML", "ZenScript", "Zephir"
};

